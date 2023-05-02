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
	while(str[++i])
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
	while(++i < num_cmd - 1)
		free(fdlist[i]);
	free(fdlist);
	data->fd = NULL;
}

void	free_targ(t_cmd *cmd)
{
	int	i;
	t_arg *arg;

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
	int	i;
	t_cmd *cmd;

	if (!data->cmd)
		return ;
	cmd = data->cmd;
	i = -1;
	while (++i < data->num_cmd)
	{
		if (cmd[i].str)
			free(cmd[i].str);
		free_targ(&cmd[i]);
		free_strlist(cmd[i].array_arg);
		free_strlist(cmd[i].hd_array);
		free_strlist(cmd[i].file);
		// if (cmd[i].path)
		// 	free(cmd[i].path);
		if (cmd[i].redir)
			free(cmd[i].redir);
		if (cmd[i].fd_array)
			free(cmd[i].fd_array);
		if (cmd[i].heredoc_str)
			free(cmd[i].heredoc_str);
	}
	free(cmd);
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
	free_cmd(data);
	free_fdlist(data);
	if (data->pid)
	{
		free(data->pid);
		data->pid = NULL;
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
	// close(data->defin);
	// close(data->defout);
}