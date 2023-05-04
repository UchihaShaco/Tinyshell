/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 05:12:31 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 05:12:31 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_one_file(t_cmd *cmd, int i, int proc, t_data *data)
{
	if (cmd->redir[i] == 2)
	{
		cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
		if (cmd->fd_array[i] == -1)
			if (err_open(cmd, proc, data))
				return (1);
	}
	else if (cmd->redir[i] == 3)
	{
		cmd->fd_array[i] = open(cmd->file[i], \
		O_CREAT | O_RDWR | O_TRUNC, 0666);
		if (cmd->fd_array[i] == -1)
			if (err_open(cmd, proc, data))
				return (1);
	}
	else if (cmd->redir[i] == 4)
	{
		cmd->fd_array[i] = open(cmd->file[i], \
		O_CREAT | O_RDWR | O_APPEND, 0666);
		if (cmd->fd_array[i] == -1)
			if (err_open(cmd, proc, data))
				return (1);
	}
	else if (cmd->redir[i] == 5)
		redir5(cmd, data, i);
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
	return (0);
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
		ts_dup2(data->fd[index - 1][0], STDIN_FILENO, data);
	else if (cmd->last_input > -1)
		ts_dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO, data);
	if (cmd->last_output == -1 && index != data->num_cmd - 1)
		ts_dup2(data->fd[index][1], STDOUT_FILENO, data);
	else if (cmd->last_output > -1)
		ts_dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO, data);
}

int	find_path_separator(t_cmd *cmd)
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
	check_as_command(cmd, CHILD, data);
}

int	exec_one_builtin(t_cmd *cmd, char * line, t_data *data)
{
	int	status;

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
	if (sig == SIGQUIT)
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
		data->num_prev_error = exec_one_builtin(&data->cmd[0], line, data);
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
