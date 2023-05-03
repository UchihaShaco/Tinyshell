#include "minishell.h"

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
int	err(char *str, int errno, int proc, t_data *data)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	strerror(errno);
	close_pipes(data);
	if (proc == CHILD)
		exit(1);
	else
	{
		close(data->defin);
		close(data->defout);
		return (1);
	}
	return (0);

}
int	open_one_file(t_cmd *cmd, int i, int proc, t_data *data)
{
	if (cmd->redir[i] == 2)
	{
		cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
		if (cmd->fd_array[i] == -1)
		{
			// put_strs_fd(3, data, 2, "bash: ", cmd->file[i], strerror(errno));
			ft_putstr_fd(strerror(errno), 2);
			close_fd_array(cmd, data);
			close_pipes(data);
			if (proc == CHILD)
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
		{
			// put_strs_fd(3, data, 2, "bash: ", cmd->file[i], strerror(errno));
			ft_putstr_fd(strerror(errno), 2);
			close_fd_array(cmd, data);
			close_pipes(data);
			if (proc == CHILD)
				exit (EXIT_FAILURE);
			else
			{
				close(data->defin);
				close(data->defout);
				return (1);
			}
		}
			// error(ERR_OPEN, data);
	}
	else if (cmd->redir[i] == 4)
	{
		cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
		if (cmd->fd_array[i] == -1)
		{
			// put_strs_fd(3, data, 2, "bash: ", cmd->file[i], strerror(errno));
			ft_putstr_fd(strerror(errno), 2);
			close_fd_array(cmd, data);
			close_pipes(data);
			if (proc == CHILD)
				exit (EXIT_FAILURE);
			else
			{
				close(data->defin);
				close(data->defout);
				return (1);
			}
		}
			// error(ERR_OPEN, data);
	}
	else if (cmd->redir[i] == 5)
	{
		if (cmd->last_input == i)
			get_heredoc_fd(cmd, data);
		else 
			cmd->fd_array[i] = -2;
	}
	return (0);
}

/* proc is either CHILD or PARENT. returns 1 if there is restriction, 0 if there isn't
check file permissions already change exit value num*/
int	open_files(t_cmd *cmd, int proc, char *line, t_data *data)
{
	int	i;
	int	exit_val;

	if (cmd->count_redir == 0)
		return (0);
	cmd->fd_array = (int *)ts_calloc(cmd->count_redir, sizeof(int), data);
	i = -1;
	while (++i < cmd->count_redir)
	{
		exit_val = open_one_file(cmd, i, proc, data);
		if (exit_val == 1)
			return (1);
	}
	return(0);
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

int		find_path_separator(t_cmd *cmd)
{
	int		i;
	char	*arg;

	i = -1;
	arg = cmd->array_arg[0];
	while (arg[++i])
		if (arg[i] == '/')
			return (1);
	return (0);
}

/*
look for a path separator
if path separator exists, check if cmd is a directory
otherwise check if it is a command 
*/
void	check_as_dir(t_cmd *cmd, int proc, t_data *data)
{
	struct stat file_stat;
	int			stat_res;

	if (!find_path_separator(cmd) && data->env_paths)
		return ;
	stat_res = stat(cmd->array_arg[0], &file_stat);
	/* if file doesn't exist */
	if (stat_res == -1)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": No such file or directory\n");
		exit(127);
	}
	/* if file exists and is a directory */
	else if (S_ISDIR(file_stat.st_mode))
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": Is a directory\n");
		exit (126);
	}
	/* file exists and is a regular file , then execute if possible */
	else if (S_ISREG(file_stat.st_mode))
	{
		/* check permissions */
		if (!(file_stat.st_mode & S_IXUSR))
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": Permission denied\n");
			data->num_prev_error = 126;
			exit (126);
		}
		if (execve(cmd->array_arg[0], cmd->array_arg, data->our_env) == -1)
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": No such file or directory\n");
			data->num_prev_error = 127;
			exit(127);
		}
	}
}

int	check_permissions_executable(t_cmd *cmd, t_data *data)
{
	struct stat file_stat;
	int			stat_res;
	int			permission;

	stat_res = stat(cmd->array_arg[0], &file_stat);
	// if (stat_res == -1)
	// {
	// 	put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
	// 	exit(127);
	// }
	if (stat_res != -1 && (!(file_stat.st_mode & S_IXUSR)))
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": Permission denied\n");
		exit(126);
	}
	return(0);
}

void	check_as_command(t_cmd *cmd, int proc, t_data *data)
{
	struct stat file_stat;
	int			stat_res;

	stat_res = stat(cmd->array_arg[0], &file_stat);

	get_cmd_path(cmd, data);
	if (cmd->path == NULL)
	{
		if (access(cmd->array_arg[0], F_OK) || S_ISDIR(file_stat.st_mode)) // if it's not a file or it's a directory
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
			exit (127);
		}
		else if (access(cmd->array_arg[0], X_OK)) //it's a file but it's not executable
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": Permission denied\n");
			exit(126);
		}
		// if (access(cmd->array_arg[0], F_OK) == 0)
		// 	check_permissions_executable(cmd, data); //this will write an error message and exit if permission is denied
		// else
		// {
			// put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
			// exit(127);
		// }
	}
	if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
	{
		// put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
		ft_putstr_fd(strerror(errno), 2);
		exit(127);
	}
	
	// if (access(cmd->array_arg[0], F_OK) != 0 && !path)
	// {
	// 	put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
	// 	exit(127);
	// }
	// if (execve(path, cmd->array_arg, data->our_env) == -1)
	// {
	// 	check_permissions_executable(cmd, data);
	// 	put_strs_fd(3, data, 2, "bash: ", cmd->path, ": command not found\n");
	// 	exit(127);
	// }
}
/*
if there is 0 num arg but some redirections 
if the command doesn't exist
run cmd normally
*/
void	child_process(int i, t_cmd *cmd, char *line, t_data *data)
{
	close(data->defin);
	close(data->defout);
	open_files(cmd, CHILD, line, data);
	pipe_cmd(i, cmd, data);
	close_fd_array(cmd, data);
	close_pipes(data);
	if (cmd->num_arg == 0)
		exit(0);
	if (cmd->builtin > 0)
		execute_builtin(cmd, CHILD, line, data);
	check_as_dir(cmd, CHILD, data);
	/* check non / strings here
	-if data->paths
		check for path in the traditional way
	-if no data->paths
		check as a directory
	*/
	check_as_command(cmd, CHILD, data);
	
}

int	exec_one_builtin(t_cmd *cmd, char * line, t_data *data)
{
	int status;
	
	if (open_files(cmd, PARENT, line, data) == 1)
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

void qhandler(int sig)
{
	if(sig == SIGQUIT)
	{
	}
}
/* set exit status here */
void	execute(char *line, t_data *data)
{
	int	i;
	int	status;

	if (data->num_cmd == 0)
		data->num_prev_error = 0;
	else if (data->num_cmd == 1 && data->cmd->builtin > 0)
		data->num_prev_error =  exec_one_builtin(&data->cmd[0], line, data);
	else
	{
		i = -1;
		while (++i < data->num_cmd - 1)
			if (pipe(data->fd[i]) == -1)
				error(ERR_PIPE, data);
		i = -1;
		while (++i < data->num_cmd)
		{
			data->pid[i] = fork();
			if (data->pid[i] == -1)
				error(ERR_FORK, data);
			else if (data->pid[i] == 0)
			{
				signal(SIGQUIT, qhandler);
				child_process(i, &data->cmd[i], line, data);
			}
		}
		status = parent_process(data);
		data->num_prev_error = WEXITSTATUS(status);
	}
}
