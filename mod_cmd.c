/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 06:31:47 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 06:31:47 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* modifies hd_array, record_hd, fd_array (if last input is hd); 
inits hd in terminal */
void	init_heredoc(t_cmd *cmd, t_data *data)
{
	int	i;
	int	j;

	cmd->hd_array = ts_calloc(cmd->count_hd + 1, sizeof(char *), data);
	i = -1;
	j = -1;
	while (++i < cmd->count_redir)
		if (cmd->redir[i] == 5)
			cmd->hd_array[++j] = ft_strdup_lim(cmd->file[i], '\0', data);
	get_heredoc_str(cmd, data, 0);
}

/* iterates through redir array and if there is a double, 
turns the redir # to -1 */
void	get_cmd_path(t_cmd *cmd, t_data *data)
{
	int		i;
	char	*arg;

	i = 0;
	arg = cmd->array_arg[0];
	if (!data->env_paths)
		return ;
	while (data->env_paths[i])
	{
		cmd->path = ft_strjoin_char(data->env_paths[i], arg, '/', data);
		if (access(cmd->path, F_OK) == 0 && access(cmd->path, X_OK) == 0)
			return ;
		else if (access(cmd->path, F_OK) == 0 && access(cmd->path, X_OK) < 0)
		{
			free(cmd->path);
			cmd->path = NULL;
			return ;
		}
		free(cmd->path);
		cmd->path = NULL;
		i++;
	}
}

char	**make_new_array_arg(int new_num_arg, t_cmd *cmd, t_data *data)
{
	char	**new_array_arg;
	int		i;
	int		j;

	new_array_arg = (char **)ts_calloc(new_num_arg + 1, \
	sizeof(char *), data);
	i = -1;
	j = 0;
	while (cmd->array_arg[++i])
	{
		if (cmd->array_arg[i][0] != '\0')
		{
			new_array_arg[j] = ft_strdup_lim(cmd->array_arg[i], '\0', data);
			j++;
		}
	}
	return (new_array_arg);
}
/* check if there is $EMPTY in cmd->array_arg 
if nothing remains in array_arg, */

void	check_empty(t_cmd *cmd, t_data *data, int i, int count_empty)
{
	int		new_num_arg;
	char	**new_array_arg;

	new_array_arg = NULL;
	if (!cmd->array_arg)
		return ;
	while (cmd->array_arg[++i])
		if (cmd->array_arg[i][0] == '\0')
			count_empty++;
	if (count_empty == 0)
	{
		cmd->old_num_arg = cmd->num_arg;
		return ;
	}
	new_num_arg = cmd->num_arg - count_empty;
	if (new_num_arg > 0)
		new_array_arg = make_new_array_arg(new_num_arg, cmd, data);
	free_strlist(cmd->array_arg);
	cmd->array_arg = NULL;
	cmd->array_arg = new_array_arg;
	cmd->old_num_arg = cmd->num_arg;
	cmd->num_arg = new_num_arg;
}

void	finalize_cmd(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_cmd)
	{
		if (data->cmd[i].count_redir > 1)
			check_redir_doubles(&data->cmd[i], data);
		check_hd_last_redir(&data->cmd[i], data);
		if (data->cmd[i].count_hd > 0)
			init_heredoc(&data->cmd[i], data);
		if (g_hdsig == 42)
			break ;
		check_empty(&data->cmd[i], data, -1, 0);
		if (data->cmd[i].num_arg > 0)
			data->cmd[i].builtin = check_builtin(&data->cmd[i], data);
	}
}
