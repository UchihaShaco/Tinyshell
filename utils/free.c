/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 03:40:32 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 03:40:32 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* free an array of strings */
void	free_strlist(char **str)
{
	int	i;

	if (!str)
		return ;
	if (!*str)
	{
		free(str);
		return ;
	}
	i = -1;
	while (str[++i])
	{
		free(str[i]);
	}
	free(str);
}

void	free_fdlist(t_data *data)
{
	int	i;
	int	**fdlist;
	int	num_cmd;

	fdlist = data->fd;
	num_cmd = data->num_cmd;
	if (!fdlist)
		return ;
	if (!*fdlist)
	{
		free(fdlist);
		return ;
	}
	i = -1;
	while (++i < num_cmd - 1)
		free(fdlist[i]);
	free(fdlist);
	data->fd = NULL;
}

void	free_targ(t_cmd *cmd)
{
	t_arg	*arg;
	int		i;

	arg = cmd->arg;
	if (!arg)
		return ;
	i = -1;
	while (++i < cmd->old_num_arg + 1)
	{
		if (arg[i].str)
			free(arg[i].str);
	}
	free(arg);
	cmd->arg = NULL;
}

void	free_cmd(t_data *data)
{
	int		i;

	if (!data->cmd)
		return ;
	i = -1;
	while (++i < data->num_cmd)
	{
		if (data->cmd[i].str)
			free(data->cmd[i].str);
		free_targ(&data->cmd[i]);
		free_strlist(data->cmd[i].array_arg);
		free_strlist(data->cmd[i].hd_array);
		free_strlist(data->cmd[i].file);
		if (data->cmd[i].path)
			free(data->cmd[i].path);
		if (data->cmd[i].redir)
			free(data->cmd[i].redir);
		if (data->cmd[i].fd_array)
			free(data->cmd[i].fd_array);
		if (data->cmd[i].heredoc_str)
			free(data->cmd[i].heredoc_str);
	}
	free(data->cmd);
	data->cmd = NULL;
}

void	free_data(t_data *data, char *line, int last)
{
	if (last == YES)
	{
		clear_envlist(data->env_list);
		data->env_list = NULL;
		free_strlist(data->our_env);
		data->our_env = NULL;
		free_strlist(data->env_paths);
		data->env_paths = NULL;
		if (data->cur_dir)
		{
			free(data->cur_dir);
			data->cur_dir = NULL;
		}
		if (data->old_dir)
		{
			free(data->old_dir);
			data->old_dir = NULL;
		}
	}
	free_util(data, line);
}
