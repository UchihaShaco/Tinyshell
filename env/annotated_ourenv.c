#include "minishell.h"


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
	if (env_var->val)
		ft_strlcat(new_entry, env_var->val, key_len + val_len + 2);
	return (new_entry);
}

/* free our_env and rewrite the whole array */
void	rewrite_ourenv(t_data *data)
{
	int		i;
	t_env	*cur;

	if (data->our_env)
		free_strlist(data->our_env);
	i = 0;
	cur = *data->env_list;
	/* find number of env_list nodes that have = signs. only those can be added to env */
	while (cur)
	{
		if (cur->equal == 1)
			i++;
		cur = cur->next;
	}
	/* create new env array and fill it */
	data->our_env = (char **)ts_calloc(i + 1, sizeof(char *), data);
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


