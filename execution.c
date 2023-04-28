#include "minishell.h"

/* write null to pipe */
void	pipe_null(int index, t_data *data)
{
	int	devnull;
	devnull = open("dev/null", O_WRONLY);
	if (devnull < 0)
		error(ERR_OPEN, data);
	ts_dup2(devnull, data->fd[index][1], data);
	close(devnull);
}

void	close_fd_array(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->count_redir)
	{
		if (i != -1)
		{
			close(cmd->fd_array[i]);
			cmd->fd_array[i] = -1;
		}
		i++;
	}
}

/* 
	if no input && not first ommand, read input from previous pipe
	if input, read input from input
	if no output && not last cmd, write results into pipe
	if output, write results into output
	close all files
*/
void	pipe_cmd(int index, t_cmd *cmd, t_data *data)
{
	if (cmd->last_input == -1 && index > 0)
	{
		// printf("no input and not first command\n");
		ts_dup2(data->fd[index - 1][0], STDIN_FILENO, data);
	}
	else if (cmd->last_input > -1)
		ts_dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO, data);
	if (cmd->last_output == -1 && index != data->num_cmd - 1)
	{
		// printf("no output redirection and not last cmd\n");
		ts_dup2(data->fd[index][1], STDOUT_FILENO, data);
	}
	else if (cmd->last_output > -1)
	{
		ts_dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO, data);
		// if (index != data->num_cmd - 1)
		// 	pipe_null(index, data);
	}
	close_fd_array(cmd);
		
}

void	child_process(int i, t_cmd *cmd, t_data *data)
{
	int	j;

	pipe_cmd(i, cmd, data);
	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
	}
	if (cmd->builtin > 0)
		execute_builtin(cmd, NO, data);
	else
	{
		if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
			error(ERR_EXEC, data);
	}
}

int	parent_process(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->num_cmd - 1)
	{
		close(data->fd[i][0]);
		close(data->fd[i][1]);
		i++;
	}
	i = 0;
	while (i < data->num_cmd)
	{
		waitpid(data->pid[i], &status, 0);
		i++;
	}
	return (status);
}

void	exec_one_builtin(t_cmd *cmd, t_data *data)
{
	int fd;
	/* redirect */
	if (cmd->last_input > -1)
		ts_dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO, data);
	if (cmd->last_output > -1)
		ts_dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO, data);
	close_fd_array(cmd);
	execute_builtin(&data->cmd[0], YES, data);
}

int	execute(t_data *data)
{
	int	i;
	int	status;

	if (data->num_cmd == 1 && data->cmd->builtin > 0)
		exec_one_builtin(&data->cmd[0], data);
	/* create pipes and fork*/
	else
	{
		i = -1;
		/* create pipes based on number of commands - 1*/
		while (++i < data->num_cmd - 1)
			if (pipe(data->fd[i]) == -1)
				error(ERR_PIPE, data);
		i = -1;
		/* fork and run the child process */
		while (++i < data->num_cmd)
		{
			data->pid[i] = fork();
			if (data->pid[i] == -1)
				error(ERR_FORK, data);
			else if (data->pid[i] == 0)
				child_process(i, &data->cmd[i], data);
		}
		/* run the parent process */
		status = parent_process(data);
		return (WEXITSTATUS(status));
	}
	return (0);
}
