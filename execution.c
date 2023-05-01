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

void	close_fd_array(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->count_redir == 0)
		return ;
	i = -1;
	while (++i < cmd->count_redir)
		if (cmd->fd_array[i] > 0 && close(cmd->fd_array[i]) == -1)
			error(ERR_CLOSE, data);
}

void	close_pipes(t_data *data)
{
	int j;

	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
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
}

int	open_files(t_cmd *cmd, t_data *data, int one_func)
{
	int	i;

	if (cmd->count_redir == 0)
		return (0);
	cmd->fd_array = (int *)ts_calloc(cmd->count_redir, sizeof(int), data);
	i = -1;
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] == 2)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
			if (cmd->fd_array[i] == -1)
			{
				put_strs_fd(3, data, 2, "bash: ", cmd->file[i], ": No such file or directory\n");
				close_fd_array(cmd, data);
				close_pipes(data);
				if (one_func == NO)
					exit (EXIT_FAILURE);
				else
				{
					close(data->defin);
					close(data->defout);
					return (1);
				}
			}
		}
		else if (cmd->redir[i] == 3)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
			if (cmd->fd_array[i] == -1)
				error(ERR_OPEN, data);
		}
		else if (cmd->redir[i] == 4)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
			if (cmd->fd_array[i] == -1)
				error(ERR_OPEN, data);
		}
		else if (cmd->redir[i] == 5)
		{
			if (cmd->last_input == i)
				get_heredoc_fd(cmd, data);
			else 
				cmd->fd_array[i] = -2;
		}
	}
	return(0);
}

void	check_if_dir(t_cmd *cmd, char *line, t_data *data)
{
	DIR *dir;
	int	dir_exists;

	dir_exists = 0;
	dir = opendir(cmd->path);
	if (dir)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->path, ": Is a directory\n");
		dir_exists = 1;
	}
	closedir(dir);
	if (dir_exists == 1)
	{
		free_data(data, line, NO);
		exit (126);
	}
}

/*
if there is 0 num arg but some redirections 
if the command doesn't exist
run cmd normally
*/
void	child_process(int i, t_cmd *cmd, char *line, t_data *data)
{
	open_files(cmd, data, NO);
	pipe_cmd(i, cmd, data);
	close_fd_array(cmd, data);
	close_pipes(data);
	int j;

	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
	}
	if (cmd->num_arg == 0)
		exit(0);
	if (cmd->builtin > 0)
		execute_builtin(cmd, CHILD, line, data);
	if (!cmd->path)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
		exit(127);
	}
	check_if_dir(cmd, line, data);
	if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->path, ": command not found\n");
		exit(127);
		// error(ERR_EXEC, data);
	}
}

int	parent_process(t_data *data)
{
	int	i;
	int	status;

	close_pipes(data);
	i = 0;
	while (i < data->num_cmd)
	{
		waitpid(data->pid[i], &status, 0);
		i++;
	}
	return (status);
}

int	exec_one_builtin(t_cmd *cmd, char * line, t_data *data)
{
	int status;
	
	if (open_files(cmd, data, YES) == 1)
		return (1);
	if (cmd->last_input > -1)
	{
		ts_dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO, data);
	}
	if (cmd->last_output > -1)
	{
		ts_dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO, data);
	}
	close_fd_array(cmd, data);
	status = execute_builtin(&data->cmd[0], PARENT, line, data);
	return (status);
}

void	execute(char *line, t_data *data)
{
	int	i;
	int	status;

	if (data->num_cmd == 0)
		data->num_prev_error = 0;
	/* if there is one cmd and it's a builtin*/
	else if (data->num_cmd == 1 && data->cmd->builtin > 0)
	{
		status = exec_one_builtin(&data->cmd[0], line, data);
		// return (status);
		data->num_prev_error = status;
	}
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
				child_process(i, &data->cmd[i], line, data);
		}
		/* run the parent process */
		status = parent_process(data);
		data->num_prev_error = WEXITSTATUS(status);
		//in execute, update data->num_prev_error to final exit status
	}
	// return (0);
}

// int	j;

// 	pipe_cmd(i, cmd, data);
// 	j = 0;
// 	while (j < data->num_cmd - 1)
// 	{
// 		close(data->fd[j][0]);
// 		close(data->fd[j][1]);
// 		j++;
// 	}
// 	if (cmd->builtin > 0)
// 		execute_builtin(cmd, NO, data);
// 	else
// 	{
// 		if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
// 		{
// 			put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
