/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 02:24:22 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 11:55:48 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_new_var(char **split_arg, int *rewr, t_data *data)
{
	t_env	*node;

	if (!split_arg[1] && (ft_strcmp(split_arg[0], "OLDPWD") == 0 \
	|| ft_strcmp(split_arg[0], "PWD") == 0))
		node = create_pwd_envlist(split_arg, data);
	else
		node = create_var_envlist(split_arg, data);
	add_var_envlist(node, data);
	if (split_arg[1])
		rewr[0]++;
	if (ft_strcmp(split_arg[0], "PATH"))
		rewr[1]++;
}

void	update_var(char **split_arg, t_env *env_var, int *rewr, t_data *data)
{
	env_var->equal = 1;
	if (ft_strcmp(env_var->val, split_arg[2]) != 0)
	{
		if (env_var->val)
			free(env_var->val);
		env_var->val = NULL;
		if (split_arg[2])
			env_var->val = ft_strdup_lim(split_arg[2], '\0', data);
		if (ft_strcmp(env_var->key, "PATH") == 0)
			rewr[1]++;
		rewr[0]++;
	}
}

/*if (!env_var) its for if no var found
  else if (env_var && split_arg[1]) its for if
  var found and there is an equal sign
  */
void	exp_norm(char *arg, int *rewr, t_data *data)
{
	t_env	*env_var;
	char	**split_arg;

	split_arg = split_var_envlist(arg, data, NO);
	if (ft_strcmp(split_arg[0], "_") != 0)
	{
		env_var = find_var_envlist(split_arg[0], data);
		if (!env_var)
			add_new_var(split_arg, rewr, data);
		else if (env_var && split_arg[1])
			update_var(split_arg, env_var, rewr, data);
	}
	free_strlist(split_arg);
}

void	exp_error(char *arg, int *exit_val, t_data *data)
{
	put_strs_fd(3, data, 2, "TinyShell: export: '", \
	arg, "': not a valid identifier\n");
	*exit_val = 1;
}

int	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	int		*rewr;
	int		status;
	int		exit_val;

	exit_val = 0;
	rewr = ts_calloc(2, sizeof(int), data);
	if (!cmd->array_arg[1])
		return (free(rewr), print_export(data), 0);
	i = 0;
	while (cmd->array_arg[++i])
	{
		status = check_exp_entry(cmd->array_arg[i]);
		if (status == 2)
			exp_error(cmd->array_arg[i], &exit_val, data);
		else if (status == 1)
			exp_concat(cmd->array_arg[i], rewr, data);
		else if (status == 0)
			exp_norm(cmd->array_arg[i], rewr, data);
	}
	if (rewr[0] > 0)
		rewrite_ourenv(data);
	if (rewr[1] > 0)
		get_env_paths(data);
	return (free(rewr), exit_val);
}
