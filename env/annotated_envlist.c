/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   annotated_envlist.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 02:04:22 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 02:15:10 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* add node to the envlist */

void	add_var_envlist(t_env *node, t_data *data)
{
	t_env	*last;

	if (!*data->env_list)
		*data->env_list = node;
	else
	{
		last = *data->env_list;
		while (last->next)
			last = last->next;
		last->next = node;
		node->prev = last;
	}
}
/* split up a given string at = to get 
[key, =, val]. May not have = and/or val 
if no = or val, calloc 2 spaces
else calloc 4 spaces even if there is no value
*/

char	**split_var_envlist(char *str, t_data *data)
{
	int		i;
	char	**split_list;

	i = detect_char(str, '=');
	if (i == -1)
	{
		split_list = (char **)ts_calloc(2, sizeof(char *), data);
		split_list[0] = ft_strdup_lim(str, '\0', data);
	}
	else
	{
		split_list = (char **)ts_calloc(4, sizeof(char *), data);
		split_list[0] = ft_strdup_lim(str, '=', data);
		split_list[1] = ft_strdup_lim("=", '\0', data);
		str += i + 1;
		if (str)
			split_list[2] = ft_strdup_lim(str, '\0', data);
	}
	return (split_list);
}

/* manually create an oldpwd var with no = or val if one doesn't exist */

void	add_oldpwd(t_data *data, t_env **cur)
{
	t_env	*node;

	if (find_var_envlist("OLDPWD", data) == NULL)
	{
		node = (t_env *)ts_calloc(1, sizeof(t_env), data);
		node->key = ft_strdup_lim("OLDPWD", '\0', data);
		node->prev = *cur;
		*cur->next = node;
	}
}

/* init envlist
the if statement to not print out _ var in export 
*/

static void	init_while(t_data *data, char ***split_var, \
t_env **node, t_env **cur)
{
	if (ft_strcmp((*node)->key, "_") == 0)
		(*node)->p = 1;
	(*node)->prev = *cur;
	free_strlist(*split_var);
	if (!*data->env_list)
		*data->env_list = *node;
	else
		(*cur)->next = *node;
	*cur = *node;
}

void	init_envlist(t_data *data, char **envp)
{
	int		i;
	t_env	*cur;
	t_env	*node;
	char	**split_var;

	if (!envp || !*envp)
		return ;
	data->env_list = (t_env **)ts_calloc(1, sizeof(t_env *), data);
	cur = NULL;
	i = -1;
	while (envp[++i])
	{
		split_var = split_var_envlist(envp[i], data);
		node = create_var_envlist(split_var, data);
		init_while(data, &split_var, &node, &cur);
	}
	add_oldpwd(data, &cur);
}
