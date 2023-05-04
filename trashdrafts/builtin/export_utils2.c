/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 02:42:33 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 02:49:28 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*find_next_print(t_data *data)
{
	t_env	*cur;
	t_env	*to_print;

	cur = *(data->env_list);
	while (cur && cur->p == 1)
		cur = cur->next;
	if (cur == NULL)
		return (NULL);
	to_print = cur;
	cur = cur->next;
	while (cur != NULL)
	{
		if (ft_strcmp(to_print->key, cur->key) > 0 && cur->p == 0)
			to_print = cur;
		cur = cur->next;
	}
	to_print->p = 1;
	return (to_print);
}

t_env	*create_pwd_envlist(char **var, t_data *data)
{
	t_env	*node;

	node = (t_env *)ts_calloc(1, sizeof(t_env), data);
	node->key = ft_strdup_lim(var[0], '\0', data);
	if (ft_strcmp(var[0], "OLDPWD") == 0 && data->old_dir)
	{
		node->equal = 1;
		node->val = ft_strdup_lim(data->old_dir, '\0', data);
	}
	else if (ft_strcmp(var[0], "PWD") == 0 && data->cur_dir)
	{
		node->equal = 1;
		node->val = ft_strdup_lim(data->cur_dir, '\0', data);
	}
	return (node);
}
