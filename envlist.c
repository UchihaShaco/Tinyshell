#include "minishell.h"

/* 
	Need:
	1. find var function
	2. Delete var function
		-need free_tenv function
	3. Add var function
	4. split_var_envlist
	5. Init list function
		-delete _ var
		-add OLDPWD
		-also now has = flag
*/

t_env	*find_var_envlist(char *key, t_data *data)
{
	t_env *env_key;

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
	/* only node */
	else if (delete->prev == NULL && delete->next == NULL)
		*data->env_list = NULL;
	/* first node */
	else if (delete->prev == NULL)
	{
		*data->env_list = delete->next;
		delete->next->prev = NULL;
	}
	/* last node */
	else if (delete->next == NULL)
		delete->prev->next = NULL;
	/* middle node */
	else
	{
		delete->prev->next = delete->next;
		delete->next->prev = delete->prev;
	}
	free_tenv(delete);
}

t_env	*create_var_envlist(char **var, t_data *data)
{
	t_env 	*node;
	node = (t_env *)ts_calloc(1, sizeof(t_env));
	node->key = ft_strdup_lim(var[0], '\0', data); 
	if (ft_strcmp(var[1], "=") == 0)
		node->equal = 1;
	if (var[2])
		node->val = ft_strdup_lim(var[2], '\0', data);
	return (node);
}

void	add_var_envlist(char **var, t_data *data)
{
	t_env 	*node;
	t_env	*last;
	
	node = create_var_envlist(var, data);
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

char	**split_var_envlist(char *str, t_data *data)
{
	int		i;
	char	**split_list; 

	i = detect_char(str, '=');
	/* if no = */
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

void	init_envlist(t_data *data, char **envp)
{
	int	i;
	t_env	*cur;
	t_env	*node;
	char	**split_var;

	data->env_list = (t_env **)ts_calloc(1, sizeof(t_env *), data);
	cur = NULL;
	i = -1;
	while (envp[++i])
	{
		split_var = split_var_envlist(envp[i], data);
		node = create_var_envlist(split_var, data);
		node->prev = cur;
		free_strlist(split_var);
		if (!*data->env_list)
			*data->env_list = node;
		else
			cur->next = node;
		cur = node;
	}
}