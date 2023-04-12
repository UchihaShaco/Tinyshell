#include "minishell.h"

//write dup2 function
// if (i == 0 && dup2(data->fd[i][1], STDOUT_FILENO) == -1)
	// 	error(data);
	// if (i > 0 && dup2(data->fd[i - 1][0], STDIN_FILENO) == -1)
	// 	error(data);
	// if (i == data->num_cmd - 1 && dup2(data->file2, STDOUT_FILENO) == -1)
	// 	error(data);
	// if (i != data->num_cmd - 1 && i > 0 && dup2(data->fd[i][1], STDOUT_FILENO) == -1)
	// 	error(data);

//To Do:
//fill in path for each command

void	child_process(int i, t_cmd cmd, t_data *data)
{
	int	k;
	int	j;

	// if (i == 0)
	// 	dup2(data->fd[i][1], STDOUT_FILENO);
	if (i > 0)
		dup2(data->fd[i - 1][0], STDIN_FILENO);
	if (i != data->num_cmd - 1)
		dup2(data->fd[i][1], STDOUT_FILENO);
	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
	}
	if (k = check_builtin(data->cmd->array_arg[0], data) != 0)
		execute_builtin(data->cmd->array_arg, k, data);
	else
		execve(cmd.path, cmd.array_arg, data->our_env);

	// if (execve(data->exec_list[i].path, data->exec_list[i].commands, envp) == -1)
	// 	pipex_error(EXEC_ERR, m, NULL);
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

int	pipex(t_data *data)
{
	int	i;
	int	status;

	i = -1;
	/* create pipes based on number of commands - 1*/
	while (++i < data->num_cmd - 1)
		if (pipe(data->fd[i]) == -1)
			error(data);
	i = -1;
	/* fork and run the child process */
	while (++i < data->num_cmd)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			error(data);
		else if (data->pid[i] == 0)
			child_process(i, data->cmd[i], data);
	}
	/* run the parent process */
	status = parent_process(data);
	return (WEXITSTATUS(status));
}

