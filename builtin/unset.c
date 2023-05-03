/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:35:18 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/03 22:35:18 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	invalid_unset(char *str)
{
	int	i;

	if (str[0] && !ft_isalpha(str[0]) && str[0] != '_')
		return (1);
	i = 0;
	while (str[++i])
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '#')
			return (1);
	return (0);
}

int	ft_unset(t_cmd *cmd, t_data *data)
{
	int		i;
	int		rewrite;
	int		exit_val;
	int		rewrite_paths;
	char	**arg;
	t_env	*env_var;

	i = 1;
	rewrite = 0;
	arg = cmd->array_arg;
	exit_val = 0;
	rewrite_paths = 0;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (invalid_unset(arg[i]))
		{
			put_strs_fd(3, data, 2, "bash: unset : '", arg[i], "': not a valid identifier\n");
			exit_val = 1;
		}
		else
		{
			env_var = find_var_envlist(arg[i], data);
			if (env_var)
			{
				if (strcmp(env_var->key, "OLDPWD") == 0)
				{
					free(data->old_dir);
					data->old_dir = NULL;
				}
				else if (strcmp(env_var->key, "PWD") == 0)
				{
					free(data->cur_dir);
					data->cur_dir = NULL;
				}
				if (env_var->equal == 1)
					rewrite++;
				if (strcmp(env_var->key, "PATH") == 0)
					rewrite_paths++;
				delete_var_envlist(arg[i], data);
			}
		}
		i++;
	}
	if (rewrite > 0)
		rewrite_ourenv(data);
	if (rewrite_paths > 0)
		get_env_paths(data);
	return (exit_val);
}
