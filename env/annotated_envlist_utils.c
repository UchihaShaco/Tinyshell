/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   annotated_envlist_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 02:14:33 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 02:15:31 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/* 
	1. find var function
	2. Delete functions
		-free tenv node
		-delete one node
		-clear list
	3. Add var functions
		-create node
		-add node
	4. split string by = function
	5. Init env_list function
*/

/* find var in envlist. If it exists, it returns a
to the node, if it doesn't returns NULL*/
t_env	*find_var_envlist(char *key, t_data *data)
{
	t_env	*env_key;

	if (!data->env_list || !*data->env_list)
		return (NULL);
	env_key = *(data->env_list);
	while (env_key)
	{
		if (ft_strncmp(env_key->key, key, ft_strlen(key) + 1) == 0)
			return (env_key);
		env_key = env_key->next;
	}
	return (NULL);
}

/* free a node */
void	free_tenv(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->val)
		free(node->val);
	free(node);
}

/* delete a node from the envlist
	1. only node
	2. first node
	3. last node
	4. middle node
 */
void	delete_var_envlist(char *key, t_data *data)
{
	t_env	*delete;

	delete = find_var_envlist(key, data);
	if (delete == NULL)
		return ;
	else if (delete->prev == NULL && delete->next == NULL)
		*data->env_list = NULL;
	else if (delete->prev == NULL)
	{
		*data->env_list = delete->next;
		delete->next->prev = NULL;
	}
	else if (delete->next == NULL)
		delete->prev->next = NULL;
	else
	{
		delete->prev->next = delete->next;
		delete->next->prev = delete->prev;
	}
	free_tenv(delete);
}

/* clear entire envlist */
void	clear_envlist(t_env **env_list)
{
	t_env	*cur;

	if (!env_list)
		return ;
	if (!*env_list)
	{
		free(env_list);
		return ;
	}
	cur = *env_list;
	while (cur)
	{
		if (cur->key)
			free(cur->key);
		if (cur->val)
			free(cur->val);
		*env_list = cur->next;
		free(cur);
		cur = *env_list;
	}
	free(env_list);
	env_list = NULL;
}

/* create a new node from a char ** */
t_env	*create_var_envlist(char **var, t_data *data)
{
	t_env	*node;

	node = (t_env *)ts_calloc(1, sizeof(t_env), data);
	node->key = ft_strdup_lim(var[0], '\0', data);
	if (var[1] && ft_strcmp(var[1], "=") == 0)
	{
		node->equal = 1;
		if (var[2])
			node->val = ft_strdup_lim(var[2], '\0', data);
	}
	return (node);
}
