#include "../minishell.h"

void	free_env_var(t_env *env_var)
{
	if (env_var->key)
		free(env_var->key);
	if (env_var->val)
		free(env_var->val);
	free(env_var);
}

void	remove_var(t_env *env_var, t_data *data)
{
	t_env *temp;

	temp = env_var;
	if (!env_var->prev)
		*data->env_list = env_var->next;
	else
		env_var->prev->next = env_var->next;
	if (env_var->next)
		env_var->next->prev = env_var->prev;
	free_env_var(temp);
}

void	clear_envlist(t_env **env_list)
{
	t_env *cur;


	while (cur)
	{
		cur = *env_list;
		free(cur->key);
		free(cur->val);
		*env_list = cur->next;
		free(cur);
	}
	free(env_list);
}

void	ft_unset(char **arg, t_data *data)
{
	int		i;
	t_env	*env_var;

	i = 1;
	if (!arg[i])
	{
		clear_envlist(data->env_list);
		free_strlist(data->our_env);
	}
	else
	{
		while (arg[i])
		{
			env_var = find_var_envlist(arg[i], data);
			if (env_var)
				remove_var(env_var, data);
			i++;
		}
		modify_our_env(NULL, data);
	}
}