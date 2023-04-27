#include "minishell.h"

/*
	1. find var
	2. rewrite our_env
	3. modify our_env

*/

// /* find if a variable exists in our_env. Return index if it exists, -1 if it doesn't */
// int	find_var_ourenv(char *key, t_data *data)
// {
// 	int 	i;
// 	char	*temp_key;

// 	i = 0;
// 	while (data->our_env[i])
// 	{
// 		temp_key = ft_strdup_lim(data->our_env[i], '=', data);
// 		if (ft_strncmp(temp_key, key, ft_strlen(temp_key)) == 0)
// 		{
// 			free(temp_key);
// 			return (i);
// 		}
// 		free(temp_key);
// 		i++;
// 	}
// 	free(temp_key);
// 	return (-1);
// }

/* create new entry for our_env: key=value and return the string */
char	*new_entry_ourenv(t_env *env_var, t_data *data)
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
void	rewrite_ourenv(t_data *data)
{
	int		size;
	int		i;
	t_env	*cur;

	if (data->our_env)
		free_strlist(data->our_env);
	size = 0;
	cur = *data->env_list;
	while (cur)
	{
		if (cur->equal == 1)
			size++;
		cur = cur->next;
	}
	data->our_env = (char **)ts_calloc(size + 1, sizeof(char *), data);
	i = 0;
	cur = *data->env_list;
	while (cur)
	{
		if (cur->equal == 1)
		{
			data->our_env[i] = new_entry_ourenv(cur, data);
			i++;
		}
		cur = cur->next;
	}
}

// /* modify our_env: rewrite or adjust a single variable */
// /* DO WE NEED THIS? */
// void	modify_ourenv(t_env *env_var, t_data *data) 
// {	
// 	int	i;

// 	//if env_var == NULL, we have added a new env var to the list -> rewrite entire array
// 	if (env_var == NULL)
// 		rewrite_ourenv(data);
// 	//else, we are only changing the env var to a new value -> adjust that var only
// 	else
// 	{
// 		i = find_var_ourenv(env_var->key, data);
// 		free(data->our_env[i]);
// 		data->our_env[i] = new_entry_ourenv(env_var, data);
// 	}
// }

