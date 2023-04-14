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

//remember that when freeing up t_cmd cmd, iterate through count_redir instead of going to null
// void	free_data(t_data data)
// {
// 	//free cmd
// 	while (i < cmd->count_redir)
// 		if (cmd->file[i])
// 			free(i);
	
// }

void	check_redir_doubles(t_cmd cmd, t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (i < cmd.count_redir)
	{
		while (j < cmd.count_redir)
		{
			if (cmd.file[i] && ft_strncmp(cmd.file[i], cmd.file[j], ft_strlen(cmd.file[i])) == 0)
			{
				if (cmd.redir[i] == cmd.redir[j] || (cmd.redir[i] == 3 && cmd.redir[j] == 4))
				{
					free(cmd.file[j]);
					cmd.file[j] = NULL;
					cmd.redir[j] = -1;
				}
				else if (cmd.redir[i] == 4 && cmd.redir[j] == 3)
				{
					free(cmd.file[i]);
					cmd.file[i] = NULL;
					cmd.redir[i] = -1;
				}
			}
			j++;
		}
		i++;
		j = i + 1;
	}
}

void	dup2_e(int oldfd, int newfd, t_data *data)
{
	if (dup2(oldfd, newfd) == -1)
		error(data);
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
	// < = 2 input redirection
	// > = 3 output redirection
	// >> = 4 output redirection but open file for appending
	// << = 5 heredoc

void	redirect(int index, t_cmd cmd, t_data *data)
{
	int		i;
	int		last_input;
	int		last_output;
	int		*fd_array;
	char	*delimiter_array;
	
	//if more than 1 redirection, check for double files/redir and remove
	if (cmd.count_redir > 1)
		check_redir_doubles(cmd, data);
	fd_array = (int *)ft_calloc_e(cmd.count_redir, sizeof(int), data);
	i = 0;
	last_input = -1;
	last_output = -1;
	while (i < cmd.count_redir)
	{
		//if <, open file as rdonly.
		//if error, close all other files and return (1) --> give null to next pipe
		if (cmd.redir[i] == 2)
		{
			fd_array[i] = open(cmd.file[i], O_RDONLY);
			if (fd_array[i] == -1)
			{
				//write error
				//close all other files
				while (i <= 0)
				{
					close(fd_array[i]);
					i--;
				}
				free(fd_array);
				//send null to pipe output
				if (index != data->num_cmd - 1)
					pipe_null(index, data);
				//error(data);
				return ;
			}
			//track last_input to dup2
			last_input = i;
		}
		//if >, create file and track last output to put into dup2
		else if (cmd.redir[i] == 3)
		{
			fd_array[i] = open(cmd.file[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
			if (fd_array[i] == -1)
				error(data); 
				//open error -> exit
			last_output = i;
		}
		//if >>, open file for appending and track last output to put into dup2
		else if (cmd.redir[i] == 4)
		{
			fd_array[i] = open(cmd.file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
			if (fd_array[i] == -1)
				error(data); 
				//open error -> exit
			last_output = i;
		}
		else if (cmd.redir[i] == 5)
		{
			//keep track of heredoc delimiter in an array
			//if there a heredoc redirector, will have to open up terminal for writing
			//use readline to print > everytime something is written
			//heredoc delimiters must be written IN ORDER for it to exit out
			//if more than one heredoc delimiter
			//disregard everything written until second to the last delimiter passes
			//start writing to temporary file everything after that 2nd to last delimiter
			//unlink temporary file -> in close function??? does fd_array have to be a struct???????
			//other option is to create a temporary pipe - write to pipe and have command read from read end 
		}
		else if (cmd.redir[i] == -1)
			fd_array[i] = -1;
		i++;
	}
	//if there is no < and it's not the first command, read input from previous pipe
	if (last_input == -1 && index > 0)
		dup2_e(data->fd[index - 1][0], STDIN_FILENO, data);
	//if there is a < read input from file
	else if (last_input > -1)
		dup2_e(fd_array[last_input], STDIN_FILENO, data);
	//if there is no > or >> and it is not the last command, write results into pipe
	if (last_output == -1 && index != data->num_cmd - 1)
		dup2_e(data->fd[index][1], STDOUT_FILENO, data);
	//else if there is > or >>, write results into file
	else if (last_output > -1)
	{
		dup2_e(fd_array[last_output], STDOUT_FILENO, data);
		if (index != data->num_cmd - 1)
			pipe_null(index, data);
	}
	//close all files
	i = 0;
	while (i < cmd.count_redir)
	{
		if (cmd.file[i])
			close(fd_array[i]);
		i++;
	}
		

}

//if no redirections then dup pipe ends with stdin and stdout
//if there is an input redirection only, dup stdout
//if there is an output redirection only, dup stdin
	//send null to read end of pipe
//if both input and output redirection, don't dup pipes



void	child_process(int i, t_cmd cmd, t_data *data)
{
	int	k;
	int	j;

	// if (i == 0)
	// 	dup2(data->fd[i][1], STDOUT_FILENO);
	// if (i > 0)
	// 	dup2(data->fd[i - 1][0], STDIN_FILENO);
	// if (i != data->num_cmd - 1)
	// 	dup2(data->fd[i][1], STDOUT_FILENO);
	redirect(i, cmd, data);
	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
	}
	k = check_builtin(data->cmd->array_arg[0], data);
	if (k != 0)
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

