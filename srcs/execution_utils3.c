/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 08:15:36 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/04 12:02:06 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_one_file_util(t_cmd *cmd, int i, int proc, t_data *data)
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
	return (0);
}

int	open_one_file(t_cmd *cmd, int i, int proc, t_data *data)
{
	int	ret;

	ret = 0;
	if (cmd->redir[i] == 2 || cmd->redir[i] == 3)
		ret = open_one_file_util(cmd, i, proc, data);
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
	return (ret);
}

/* proc is either CHILD or PARENT. returns 1 if there is restriction, 
0 if there isn't check file permissions already change exit value num*/

int	open_files(t_cmd *cmd, int proc, t_data *data)
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
