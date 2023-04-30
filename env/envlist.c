#include "minishell.h"

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

void	add_oldpwd(t_data *data, t_env **cur)
{
	t_env *node;

	if (find_var_envlist("OLDPWD", data) == NULL)
	{
		node = (t_env *)ts_calloc(1, sizeof(t_env), data);
		node->key = ft_strdup_lim("OLDPWD", '\0', data);
		node->prev = *cur;
		*cur->next = node;
	}
}

void	init_envlist(t_data *data, char **envp)
{
	int		i;
	t_env	*cur;
	t_env	*node;
	char	**split_var;
	
	if(!envp || !*envp)
		return ;
	data->env_list = (t_env **)ts_calloc(1, sizeof(t_env *), data);
	cur = NULL;
	i = -1;
	while (envp[++i])
	{
		split_var = split_var_envlist(envp[i], data);
		node = create_var_envlist(split_var, data);
		/* don't print out _ var in export */
		if (ft_strcmp(node->key, "_") == 0)
			node->p = 1;
		node->prev = cur;
		free_strlist(split_var);
		if (!*data->env_list)
			*data->env_list = node;
		else
			cur->next = node;
		cur = node;
	}
	add_oldpwd(data, &cur);
}