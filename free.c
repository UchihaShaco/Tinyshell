#include "minishell.h"

/* free an array of strings */
void	free_strlist(char **str)
{
	int	i;

	i = 0;
	if (str && *str)
	{
		while(str[i])
		{

			free(str[i]);
			i++;
		}
	}
	if (str)
		free(str);
	str = NULL;
}

void	free_fdlist(int **fdlist, int num_cmd)
{
	int	i;

	i = 0;
	if (!fdlist)
		return ;
	while(i < num_cmd - 1)
	{
		free(fdlist[i]);
		i++;
	}
	free(fdlist);

}

void	free_targ(t_arg *arg)
{
	if (!arg)
		return ;
	if (arg->str)
		free(arg->str);
	free(arg);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->str)
		free(cmd->str);
	free_targ(cmd->arg);
	free_strlist(cmd->array_arg);
	free_strlist(cmd->hd_array);
	free_strlist(cmd->file);
	if (cmd->path)
		free(cmd->path);
	if (cmd->redir)
		free(cmd->redir);
	if (cmd->fd_array)
		free(cmd->fd_array);
	free(cmd);
}

void	free_data(t_data *data, char *line, int last)
{
	if (last == YES)
	{
		clear_envlist(data->env_list);
		free_strlist(data->our_env);
		free_strlist(data->env_paths);
		if (data->cur_dir)
			free(data->cur_dir);
	}
	else
	{
		free_cmd(data->cmd);
		free_fdlist(data->fd, data->num_cmd);
		if (data->pid)
			free(data->pid);
	}
	if (line)
		free(line);
}