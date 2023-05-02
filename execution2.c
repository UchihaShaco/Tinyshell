#include "minishell.h"

int	handle_dir(t_cmd *cmd, int i, t_data *data)
{
	if (cmd->redir[i] == 2)
		put_strs_fd(1, data, 2, "bash: input is a directory\n");
	else if (cmd->redir[i] == 3 || cmd->redir[i] == 4)
		put_strs_fd(3, data, 2, "bash: ", cmd->file[i], ": Is a directory\n");
	return (1);
}
int	check_one_file_permissions(t_cmd *cmd, int i, t_data *data)
{
	struct stat file_stat;
	int			stat_res;
	int			permission;
	int			dir_status;

	//if heredoc, return 
	if (cmd->redir[i] == 5)
		return (0);
	stat_res = stat(cmd->file[i], &file_stat);
	//check if file is a directory
	if (stat_res != -1 && S_ISDIR(file_stat.st_mode))
	{
		dir_status = handle_dir(cmd, i, data);
		if (dir_status == 1)
			return (1);
	}
	/* if input redir
	if file doesn't exist, return 1
	find read permissions
	*/
	if (cmd->redir[i] == 2)
	{
		if (stat_res == -1)
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->file[i], ": No such file or directory\n");
			return (1);
		}
		permission = (file_stat.st_mode & S_IRUSR);
	}
	/* if output redir
	if file doesn't exist, return 0
	find write permissions */
	else if (cmd->redir[i] == 3 || cmd->redir == 4)
	{
		if (stat_res == -1)
			return (0);
		permission = (file_stat.st_mode & S_IWUSR);
	}
	if (permission)
		return (0);
	else
	{
		put_strs_fd(3, data, 3, "bash: ", cmd->file[i], ": Permission denied\n");
		return (1);
	}
	return (0);
}

/* check file permissions. If a restricted file is found, set num error and exit/return */
int	check_file_permissions(t_cmd *cmd, int proc, char *line, t_data *data)
{
	int	restrict_file;
	int	i;

	restrict_file = 0;
	i = -1; 
	while (++i < cmd->count_redir)
	{
		restrict_file = check_one_file_permissions(cmd, i, data);
		if (restrict_file == 1)
		{
			data->num_prev_error = 1;
			if (proc == CHILD)
				exit(1);
			else
				return (1);
		}
	}
	return (0);
}

/* we're going to assume that no one changed the file in between us checking the file permissions and opening them
else we would have to use stat, open, and then use fstat
or open file and then use fstat 
But I'd rather not have to open any files at all or calloc if we can check permissions first*/
void	open_one_file(t_cmd *cmd, int i, t_data *data)
{
	if (cmd->redir[i] == 2)
		cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
	else if (cmd->redir[i] == 3)
		cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
	else if (cmd->redir[i] == 4)
		cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
	else if (cmd->redir[i] == 5)
	{
		if (cmd->last_input == i)
			get_heredoc_fd(cmd, data);
		else 
			cmd->fd_array[i] = -2;
	}
}

/* proc is either CHILD or PARENT */
int	open_files(t_cmd *cmd, int proc, t_data *data)
{
	int	i;

	if (cmd->count_redir == 0)
		return (0);
	if (check_file_permissions(cmd, proc, data) == 1)
		return (1);
	cmd->fd_array = (int *)ts_calloc(cmd->count_redir, sizeof(int), data);
	i = -1;
	while (++i < cmd->count_redir)
		open_one_file(cmd, i, data);
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
void	check_dir(t_cmd *cmd, int proc, t_data *data)
{
	struct stat file_stat;
	int			stat_res;

	if (!find_path_separator(cmd))
		return ;
	stat_res = stat(cmd->array_arg[0], &file_stat)
	/* if file doesn't exist */
	if (stat_res == -1)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->path, ": No such file or directory\n");
		data->prev_num_error = 127;
		exit(127);
	}
	/* if file exists and is a directory */
	else if (S_ISDIR(file_stat.st_mode))
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->path, ": Is a directory\n");
		data->prev_num_err = 126;
		exit (126);
	}
	/* file exists and is a regular file , then execute if possible */
	else if (S_ISREG(file_stat.st_mode))
	{
		/* check permissions */
		if (!(file_stat.st_mode & S_IXUSR))
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->file[i], ": Permission denied\n");
			data->prev_num_err = 126;
			exit (126);
		}
		if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
		{
			put_strs_fd(3, data, 2, "bash: ", cmd->path, ": No such file or directory\n");
			data->prev_num_err = 127;
			exit(127);
		}
	}
}
/*
if there is 0 num arg but some redirections 
if the command doesn't exist
run cmd normally
*/
void	child_process(int i, t_cmd *cmd, char *line, t_data *data)
{
	open_files(cmd, CHILD, data);
	pipe_cmd(i, cmd, data);
	close_fd_array(cmd, data);
	close_pipes(data);
	if (cmd->num_arg == 0)
		exit(0);
	if (cmd->builtin > 0)
		execute_builtin(cmd, CHILD, line, data);
	check_dir(cmd, CHILD, data);
	if (!cmd->path)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->array_arg[0], ": command not found\n");
		data->num_prev_error = 127;
		exit(127);
	}
	if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
	{
		put_strs_fd(3, data, 2, "bash: ", cmd->path, ": command not found\n");
		data->num_prev_error = 127;
		exit(127);
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
				child_process(i, &data->cmd[i], line, data);
		}
		status = parent_process(data);
		data->num_error = WEXITSTATUS(status);
	}
}
