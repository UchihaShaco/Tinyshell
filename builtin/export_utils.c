/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 02:29:58 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 02:34:39 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_prints(t_data *data)
{
	t_env	*cur;

	cur = *(data->env_list);
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, "_") != 0)
			cur->p = 0;
		cur = cur->next;
	}
}

void	print_export(t_data *data)
{
	t_env	*cur;
	t_env	*to_print;

	if (!data->env_list)
		return ;
	while (1)
	{
		to_print = find_next_print(data);
		if (!to_print)
			break ;
		put_strs_fd(2, data, 1, "declare -x ", to_print->key);
		if (to_print->equal == 1)
		{
			put_strs_fd(1, data, 1, "=");
			if (to_print->val == NULL)
				put_strs_fd(1, data, 1, "\"\"\n");
			else
				put_strs_fd(3, data, 1, "\"", to_print->val, "\"\n");
		}
		else
			put_strs_fd(1, data, 1, "\n");
	}
	reset_prints(data);
}

/* returns 0 if good, 1 concat, 2 invalid */

int	check_exp_entry(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (2);
	while (str[++i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			if (str[i] == '+' && str[i + 1] == '=')
				return (1);
			else if (str[i] == '=')
				return (0);
			return (2);
		}
	}
	return (0);
}

void	exp_concat_val(char **split_arg, t_env *env_var, t_data *data)
{
	char	*new_val;
	int		len;

	if (!env_var->val)
		env_var->val = ft_strdup_lim(split_arg[2], '\0', data);
	else
	{
		len = ft_strlen(env_var->val) + ft_strlen(split_arg[2]) + 1;
		new_val = (char *)ts_calloc(len, sizeof(char), data);
		ft_strlcpy(new_val, env_var->val, len);
		ft_strlcat(new_val, split_arg[2], len);
		if (env_var->val)
			free(env_var->val);
		env_var->val = new_val;
	}
}

/* rewr[0] = ourenv, rewr[1] = envpaths */
/* else found existing var in list 
 */
void	exp_concat(char *str, int *rewr, t_data *data)
{
	t_env	*env_var;
	t_env	*node;
	char	**split_arg;

	split_arg = split_var_envlist(str, data, YES);
	env_var = find_var_envlist(split_arg[0], data);
	rewr[0]++;
	if (ft_strcmp(split_arg[0], "PATH") == 0)
		rewr[1]++;
	if (!env_var)
	{
		node = create_var_envlist(split_arg, data);
		add_var_envlist(node, data);
		rewr[0]++;
	}
	else
	{
		if (!split_arg[2])
			env_var->equal = 1;
		else
			exp_concat_val(split_arg, env_var, data);
	}
	free_strlist(split_arg);
}
