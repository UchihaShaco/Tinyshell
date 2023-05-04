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

void	env_var_check(t_data *data, t_env **env_var, \
int *rewrite_paths, int *rewrite)
{
	if (strcmp((*env_var)->key, "OLDPWD") == 0)
	{
		free(data->old_dir);
		data->old_dir = NULL;
	}
	else if (strcmp((*env_var)->key, "PWD") == 0)
	{
		free(data->cur_dir);
		data->cur_dir = NULL;
	}
	if ((*env_var)->equal == 1)
		(*rewrite)++;
	if (strcmp((*env_var)->key, "PATH") == 0)
		(*rewrite_paths)++;
}

void	unset_error(int *exit_val, char *str, t_data *data)
{
	*exit_val = 1;
	put_strs_fd(3, data, 2, "bash: unset :'", \
	str, "': not a valid identifier\n");
}

void	unset_init(int *i, int *rewrite, int *exit_val, int *rewrite_paths)
{
	*i = 0;
	*rewrite = 0;
	*exit_val = 0;
	*rewrite_paths = 0;
}

int	ft_unset(t_cmd *cmd, t_data *data, int i)
{
	int		rewrite;
	int		rewrite_paths;
	int		exit_val;
	t_env	*env_var;

	unset_init(&i, &rewrite, &exit_val, &rewrite_paths);
	while (cmd->array_arg[++i])
	{
		if (invalid_unset(cmd->array_arg[i]))
			unset_error(&exit_val, cmd->array_arg[i], data);
		else
		{
			env_var = find_var_envlist(cmd->array_arg[i], data);
			if (env_var)
			{
				env_var_check(data, &env_var, &rewrite_paths, &rewrite);
				delete_var_envlist(cmd->array_arg[i], data);
			}
		}
	}
	if (rewrite > 0)
		rewrite_ourenv(data);
	if (rewrite_paths > 0)
		get_env_paths(data);
	return (exit_val);
}
