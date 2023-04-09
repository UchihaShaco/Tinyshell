#include "minishell.h"

//find if a variable exists and return the pointer to the variable if it exists. If not return NULL
t_env	*find_var_envlist(char *key, t_data *data)
{
	t_env *env_key;

	env_key = *(data->env_list);
	while (env_key)
	{
		if (ft_strncmp(env_key->key, key, ft_strlen(key) + 1) == 0)
			return (env_key);
		env_key = env_key->next;
	}
	return (NULL);
}

/* find if a variable exists in our_env. Return index if it exists, -1 if it doesn't */
int	find_var_ourenv(char *key, t_data *data)
{
	int 	i;
	char	*temp_key;

	i = 0;
	while (data->our_env[i])
	{
		temp_key = ft_strdup_lim(data->our_env[i], '=', data);
		if (ft_strncmp(temp_key, key, ft_strlen(temp_key)) == 0)
		{
			free(temp_key);
			return (i);
		}
		free(temp_key);
		i++;
	}
	free(temp_key);
	return (-1);
}

/* create new entry for our_env: key=value and retrun the string */
char	*create_new_entry(t_env *env_var, t_data *data)
{
	int		key_len;
	int		val_len;
	char	*new_entry;

	key_len = ft_strlen(env_var->key);
	val_len = ft_strlen(env_var->val);
	new_entry = (char *)ft_calloc_e(key_len + val_len + 2, sizeof(char), data);
	ft_strlcpy(new_entry, env_var->key, key_len + 1);
	ft_strlcat(new_entry, "=", key_len + 2);
	ft_strlcat(new_entry, env_var->val, key_len + val_len + 2);
	return (new_entry);
}

/* free our_env and rewrite the whole array */
void	rewrite_our_env(t_data *data)
{
	int		size;
	int		i;
	t_env	*cur_env_var;
	char	**new_env;

	size = 0;
	cur_env_var = *data->env_list;
	while (cur_env_var)
	{
		size++;
		cur_env_var = cur_env_var->next;
	}
	new_env = (char **)ft_calloc_e(size + 1, sizeof(char *), data);
	i = 0;
	cur_env_var = *data->env_list;
	while (i < size)
	{
		new_env[i] = create_new_entry(cur_env_var, data);
		i++;
		cur_env_var = cur_env_var->next;
	}
	new_env[i] = NULL;
	if(data->our_env)
		free_strlist(data->our_env);
	data->our_env = new_env;
}

/* modify our_env: rewrite or adjust a single variable */
void	modify_our_env(t_env *env_var, t_data *data) 
{	
	int	i;

	//if env_var == NULL, we have added a new env var to the list -> rewrite entire array
	if (env_var == NULL)
		rewrite_our_env(data);
	//else, we are only changing the env var to a new value -> adjust that var only
	else
	{
		i = find_var_ourenv(env_var->key, data);
		free(data->our_env[i]);
		data->our_env[i] = create_new_entry(env_var, data);
	}
	
}

/* add env*/
void	add_env_var(char *key, char *val, t_data *data)
{
	t_env 	*node;
	t_env	*last;
	
	node = (t_env *)ft_calloc(1, sizeof(t_env));
	node->key = ft_strdup_lim(key, '\0', data); 
	if (!val)
		node->val = NULL;
	else
		node->val = ft_strdup_lim(val, '\0', data);
	node->next = NULL;
	last = *data->env_list; 
	while (last->next)
		last = last->next;
	last->next = node;
	node->prev = last;
	modify_our_env(NULL, data);
}

