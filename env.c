#include "minishell.h"
/* 
OUR_ENV
	-find_var_our_env
	-create_new_entry
	-rewrite_our_env
	-modify_our_env
ENV_LIST
	-find_var_envlist
	-add_env_var
	-split_env_var
	-init_env_list
ENV_PATHS
	-get_env_paths
*/

/* find if a variable exists and return the pointer to the variable if it exists. If not return NULL */
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

/* create new entry for our_env: key=value and return the string */
char	*create_new_entry(t_env *env_var, t_data *data)
{
	int		key_len;
	int		val_len;
	char	*new_entry;

	key_len = ft_strlen(env_var->key);
	val_len = ft_strlen(env_var->val);
	new_entry = (char *)ts_calloc(key_len + val_len + 2, sizeof(char), data);
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
	new_env = (char **)ts_calloc(size + 1, sizeof(char *), data);
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

/* split variable by only the first '=' and returns an array with key and value */
char	**split_env_var(char *str, t_data *data)
{
	int		var_len;
	int		val_len;
	char	**ret_split;

	ret_split = (char **)ts_calloc(3, sizeof(char *), data);
	var_len = detect_char(str, '=');
	ret_split[0] = (char *)ts_calloc(var_len + 1, sizeof(char), data);
	val_len = ft_strlen(str) - (var_len + 1);
	ret_split[1] = (char *)ts_calloc(val_len + 1, sizeof(char), data);
	ft_strlcpy(ret_split[0], str, var_len + 1);
	str += (var_len + 1);
	ft_strlcpy(ret_split[1], str, val_len + 1);
	ret_split[2] = NULL;
	return (ret_split);
}

/* init env_list */
void	init_env_list(t_data *data, char **envp)
{
	t_env	**env_list;
	t_env	*node;
	t_env	*cur;
	char	**split_var;
	int		i;

	i = -1;
	env_list = (t_env **)ts_calloc(1, sizeof(t_env *), data);
	cur = NULL;
	while (envp[++i])
	{
		node = (t_env *)ts_calloc(1, sizeof(t_env), data);
		split_var = split_env_var(envp[i], data);
		node->key = ft_strdup_lim(split_var[0], '\0', data);
		node->val = ft_strdup_lim(split_var[1], '\0', data);
		node->next = NULL;
		node->prev = cur;
		free_strlist(split_var);
		if (!*env_list)
			*env_list = node;
		else
			cur->next = node;
		cur = node;
	}
	data->env_list = env_list;
}

/* get env_paths */
void	get_env_paths(t_data *data)
{
	int		i;
	t_env	*cur;

	if (!data->env_list || !*data->env_list)
		return ;
	cur = *data->env_list;
	while (cur)
	{
		if (ft_strncmp("PATH", cur->key, 5) == 0)
			break ;
		cur = cur->next;
	}
	if (cur == NULL || cur->val == NULL)
		return ;
	//this checks for garbage values in path
	while (cur->val && *(cur->val) != '/')
		cur->val++;
	if (cur->val == NULL)
		return ;
	data->env_paths = ts_split(cur->val, ':', data);
}
