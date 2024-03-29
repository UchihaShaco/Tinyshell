/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 05:05:36 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 05:05:36 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_home_dir(t_data *data)
{
	t_env	*home;
	char	*home_path;

	home = find_var_envlist("HOME", data);
	if (!home)
		home_path = NULL;
	else
		home_path = home->val;
	return (home_path);
}

/* initiate array of fd and pid in data */
/* NOTE: must be created after data->num_cmd finalized
create a pid even if it may not be used
*/
void	create_fd_pid_array(t_data *data)
{
	int	i;

	if (data->num_cmd > 1)
	{
		data->fd = (int **)ts_calloc(data->num_cmd - 1, sizeof(int *), data);
		i = -1;
		while (++i < data->num_cmd - 1)
			data->fd[i] = (int *)ts_calloc(2, sizeof(int), data);
	}
	data->pid = (int *)ts_calloc(data->num_cmd, sizeof(int), data);
}

/* get env_paths */
void	get_env_paths(t_data *data)
{
	int		i;
	t_env	*path_node;

	i = 0;
	if (data->env_paths)
	{
		free_strlist(data->env_paths);
		data->env_paths = NULL;
	}
	path_node = find_var_envlist("PATH", data);
	if (!path_node || path_node->val == NULL)
		return ;
	while (path_node->val[i] != '\0' && path_node->val[i] != '/')
		i++;
	if (path_node->val[i] == '\0')
		return ;
	data->env_paths = ts_split(path_node->val, ':', data);
}
