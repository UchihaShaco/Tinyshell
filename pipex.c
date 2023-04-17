#include "minishell.h"

/* iterate through redirection array and look for doubles */
/* double = filename same; redir signs same; redir sign > and >>, eliminate >> */
void	check_redir_doubles(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (i < cmd->count_redir)
	{
		while (j < cmd->count_redir)
		{
			// if filenames match
			if (cmd->file[i] && ft_strncmp(cmd->file[i], cmd->file[j], ft_strlen(cmd->file[i])) == 0)
			{
				//if redir signs match, free the second 
				//if first redir sign is > and second redir sign is >>, eliminate >>
				if (cmd->redir[i] == cmd->redir[j] || (cmd->redir[i] == 3 && cmd->redir[j] == 4))
				{
					free(cmd->file[j]);
					cmd->file[j] = NULL;
					cmd->redir[j] = -1;
				}
				//if first redir sign is >> and second is >, eliminate >>
				else if (cmd->redir[i] == 4 && cmd->redir[j] == 3)
				{
					free(cmd->file[i]);
					cmd->file[i] = NULL;
					cmd->redir[i] = -1;
				}
			}
			j++;
		}
		i++;
		j = i + 1;
	}
}

//write null to pipe
void	pipe_null(int index, t_data *data)
{
	int	devnull;
	devnull = open("dev/null", O_WRONLY);
	if (devnull < 0)
		error(data);
	dup2(devnull, data->fd[index][1]);
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
/* finalize the cmd struct */
/* populate hd_array, fd_array, count_hd, fd->last_output, fd->last_input */
/* see ts_record_arr for initializiation of these values */
void	mod_cmd(t_cmd *cmd, t_data *data)
{
	int	i;
	int	hd;

	i = 0;
	hd = 0;
	//create fd_array and set all values to -1
	cmd->fd_array = (int *)ft_calloc_e(cmd->count_redir, sizeof(int), data);
	ft_memset(cmd->fd_array, -1, cmd->count_redir);
	cmd->count_hd = count_delimiters(cmd);
	//if number of heredocs is greater than 0, then calloc an hd_array
	if (cmd->count_hd > 0)
		cmd->hd_array = (char **)ft_calloc_e(cmd->count_hd + 1, sizeof(char *), data);
	while (i < cmd->count_redir)
	{
		//if <, open file as rdonly.
		//if error, that means the file doesn't exist
		//the moment a file doesn't open, nothing else is checked, null sent to command
		if (cmd->redir[i] == 2)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
			if (cmd->fd_array[i] == -1)
			{
				printf("-bash: %s: No such file or directory\n", cmd->file[i]);
				close_fd_array(cmd);
				//send null to pipe output if it's not the last command
				if (i != data->num_cmd - 1)
					pipe_null(i, data);
				//exit out of command, not sure if this is correct
				error(data); 
			}
			//track fd->last_input to dup2
			cmd->last_input = i;
		}
		//if >, create file and track last output to put into dup2
		else if (cmd->redir[i] == 3)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
			if (cmd->fd_array[i] == -1)
				error(data); //open error -> exit
			cmd->last_output = i;
		}
		//if >>, open file for appending and track last output to put into dup2
		else if (cmd->redir[i] == 4)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
			if (cmd->fd_array[i] == -1)
				error(data); //open error -> exit
			cmd->last_output = i;
		}
		//if << (heredoc), append file to delimiter array
		//there will be no file opened for this, so fd is -1 (no fd)
		else if (cmd->redir[i] == 5)
		{
			cmd->hd_array[hd] = ft_strdup_lim(cmd->file[i], '\0', data);
			hd++;
			cmd->fd_array[i] = -1;
			cmd->last_input = i;
		}
		//if this redirection was deleted, fd is -1 (no fd)
		else if (cmd->redir[i] == -1)
			cmd->fd_array[i] = -1;
		i++;
	}
}

void	redirect(int index, t_cmd *cmd, t_data *data)
{
	int		i;
	int		hd;
	int		record_hd; 
	int		fd;

	//if more than 1 redirection, check for double files/redir and remove
	if (cmd->count_redir > 1)
		check_redir_doubles(cmd);
	mod_cmd(cmd, data);
	//if last input redirection is a heredoc, record_hd = 1. Else record_hd is 0
	//record_hd means to record_hd the string in heredoc into a temp file
	record_hd = 0;
	if (cmd->last_input > -1 && cmd->redir[cmd->last_input] == 5)
		record_hd = 1;
	printf("count_hd = %i\n", cmd->count_hd);
	printf("record_hd = %i\n", record_hd);
	// print_strlist(cmd->hd_array);
	if (cmd->count_hd > 0)
		get_heredoc_fd(cmd, record_hd, data);
	//if there is no < and it's not the first command, read input from previous pipe
	if (cmd->last_input == -1 && index > 0)
		dup2_e(data->fd[index - 1][0], STDIN_FILENO, data);
	//if there is a < read input from file
	else if (cmd->last_input > -1)
		dup2_e(cmd->fd_array[cmd->last_input], STDIN_FILENO, data);
	//if there is no > or >> and it is not the last command, write results into pipe
	if (cmd->last_output == -1 && index != data->num_cmd - 1)
		dup2_e(data->fd[index][1], STDOUT_FILENO, data);
	//else if there is > or >>, write results into file
	else if (cmd->last_output > -1)
	{
		dup2_e(cmd->fd_array[cmd->last_output], STDOUT_FILENO, data);
		// if (index != data->num_cmd - 1)
		// 	pipe_null(index, data);
	}
	//close all files
	close_fd_array(cmd);
		
}

void	child_process(int i, t_cmd *cmd, t_data *data)
{
	int	k;
	int	j;

	if (cmd->count_redir > 0)
		redirect(i, cmd, data);
	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
	}
	k = check_builtin(data->cmd->array_arg[0], data);
	// printf("k: %i\n", k);
	if (k != 0)
		execute_builtin(data->cmd->array_arg, k, data);
	else
	{
		if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
			error(data);
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
			child_process(i, &data->cmd[i], data);
	}
	/* run the parent process */
	status = parent_process(data);
	return (WEXITSTATUS(status));
}
