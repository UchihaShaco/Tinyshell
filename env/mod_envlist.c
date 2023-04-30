#include "minishell.h"

void	free_tenv(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->val)
		free(node->val);
	free(node);
}

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

void	clear_envlist(t_env **env_list)
{
	t_env *cur;

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

t_env	*create_var_envlist(char **var, t_data *data)
{
	t_env 	*node;
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
