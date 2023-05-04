/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 07:06:33 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 08:31:50 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mod_redir_doubles(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (i < cmd->count_redir)
	{
		if (cmd->redir[i] != 5)
		{
			while ((cmd->redir[i] != -1) && j < cmd->count_redir)
			{
				if (ft_strcmp(cmd->file[i], cmd->file[j]) == 0)
				{
					if (cmd->redir[i] == cmd->redir[j] || \
					(cmd->redir[i] == 3 && cmd->redir[j] == 4))
						cmd->redir[j] = -1;
					else if (cmd->redir[i] == 4 && cmd->redir[j] == 3)
						cmd->redir[i] = -1;
				}
				j++;
			}
		}
		i++;
		j = i + 1;
	}
}

/* modifies cmd->file and cmd->redir - potential re-calloc*/

void	redir_utils(char ***new_f, int *new_r, t_cmd *cmd, int new_ct_redir)
{
	free_strlist(cmd->file);
	free(cmd->redir);
	cmd->file = *new_f;
	cmd->redir = new_r;
	cmd->count_redir = new_ct_redir;
}

void	make_new_arrays(t_cmd *cmd, char ***new_f, int **new_rdir, t_data *data)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] != -1)
		{
			(*new_f)[j] = ft_strdup_lim(cmd->file[i], '\0', data);
			(*new_rdir)[j] = cmd->redir[i];
			j++;
		}
	}
}

void	check_redir_doubles(t_cmd *cmd, t_data *data)
{
	int		i;
	int		j;
	int		new_count_redir;
	char	**new_file;
	int		*new_redir;

	mod_redir_doubles(cmd);
	i = -1;
	new_count_redir = 0;
	while (++i < cmd->count_redir)
		if (cmd->redir[i] != -1)
			new_count_redir++;
	if (cmd->count_redir == new_count_redir)
		return ;
	new_file = (char **)ts_calloc(new_count_redir + 1, sizeof(char *), data);
	new_redir = (int *)ts_calloc(new_count_redir, sizeof(int), data);
	make_new_arrays(cmd, &new_file, &new_redir, data);
	redir_utils(&new_file, new_redir, cmd, new_count_redir);
}

/* populates last_input, last_output, count_hd */

void	check_hd_last_redir(t_cmd *cmd, t_data *data)
{
	int	i;

	cmd->last_output = -1;
	cmd->last_input = -1;
	i = -1;
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] == 2)
			cmd->last_input = i;
		else if (cmd->redir[i] == 3)
			cmd->last_output = i;
		else if (cmd->redir[i] == 4)
			cmd->last_output = i;
		else if (cmd->redir[i] == 5)
		{
			cmd->last_input = i;
			cmd->count_hd++;
		}
	}
	if (cmd->last_input > -1 && cmd->redir[cmd->last_input] == 5)
		cmd->record_hd = 1;
}
