#include "../minishell.h"

//find next print
//print node

t_env	*find_next_print(t_data *data)
{
	t_env	*cur;
	t_env	*to_print;

	cur = *(data->env_list);
	while (cur && cur->p == 1)
		cur = cur->next;
	if (cur == NULL)
		return (NULL);
	to_print = cur;
	cur = cur->next;
	while (cur != NULL)
	{
		if (ft_strcmp(to_print->key, cur->key) > 0 && cur->p == 0)
			to_print = cur;
		cur = cur->next;
	}
	to_print->p = 1;
	return (to_print);
}

void	reset_prints(t_data *data)
{
	t_env *cur;

	cur = *(data->env_list);
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, "_") != 0)
			cur->p = 0;
		cur = cur->next;
	}
}

void	print_export(t_data *data)
{
	t_env	*cur;
	t_env	*to_print;

	if (!data->env_list)
		return ;
	while (1)
	{
		to_print = find_next_print(data);
		if (!to_print)
			break;
		put_strs_fd(2, data, 1, "declare -x ", to_print->key);
		if (to_print->equal == 1)
		{
			put_strs_fd(1, data, 1, "=");
			if (to_print->val == NULL)
				put_strs_fd(1, data, 1, "\"\"\n");
			else
				put_strs_fd(3, data, 1, "\"", to_print->val, "\"\n");
		}
		else
			put_strs_fd(1, data, 1, "\n");
	}
	reset_prints(data);
}

/* returns 0 if good, 1 concat, 2 invalid */
int	check_exp_entry(char *str)
{
	int	i;
	
	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (2);
	while (str[++i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			if (str[i] == '+' && str[i + 1] == '=')
				return (1);
			else if (str[i] == '=')
				return (0);
			return (2);
		}
	}
	return (0);
}

void	exp_concat_val(char **split_arg, t_env *env_var, t_data *data)
{
	char	*new_val;
	int		len;

	if (!env_var->val)
		env_var->val = ft_strdup_lim(split_arg[2], '\0', data);
	else
	{
		len = ft_strlen(env_var->val) + ft_strlen(split_arg[2]) + 1;
		new_val = (char *)ts_calloc(len, sizeof(char), data);
		ft_strlcpy(new_val, env_var->val, len);
		ft_strlcat(new_val, split_arg[2], len);
		if (env_var->val)
			free(env_var->val);
		env_var->val = new_val;
	}
}
/* rewr[0] = ourenv, rewr[1] = envpaths */
void	exp_concat(char *str, int *rewr, t_data *data)
{
	t_env	*env_var;
	t_env	*node;
	char **split_arg;

	split_arg = split_var_envlist(str, data, YES);
	env_var = find_var_envlist(split_arg[0], data);
	rewr[0]++;
	if (ft_strcmp(split_arg[0], "PATH") == 0)
		rewr[1]++;
	if (!env_var) //no matching var in list
	{
		node = create_var_envlist(split_arg, data);
		add_var_envlist(node, data);
		rewr[0]++;
	}
	else //found existing var in list
	{
		if (!split_arg[2]) //if no values
			env_var->equal = 1;
		else
			exp_concat_val(split_arg, env_var, data);
	}
	free_strlist(split_arg);
}

t_env	*create_pwd_envlist(char **var, t_data *data)
{
	t_env	*node;

	node = (t_env *)ts_calloc(1, sizeof(t_env), data);
	node->key = ft_strdup_lim(var[0], '\0', data);
	if (ft_strcmp(var[0], "OLDPWD") == 0 && data->old_dir)
	{
		node->equal = 1;
		node->val = ft_strdup_lim(data->old_dir, '\0', data);
	}
	else if (ft_strcmp(var[0], "PWD") == 0 && data->cur_dir) //assume that it is PWD
	{
		node->equal = 1;
		node->val = ft_strdup_lim(data->cur_dir, '\0', data);
	}
	return (node);
}

void	add_new_var(char ** split_arg, int *rewr, t_data *data)
{
	t_env *node;

	if (!split_arg[1] && (ft_strcmp(split_arg[0], "OLDPWD") == 0 || ft_strcmp(split_arg[0], "PWD") == 0))
			node = create_pwd_envlist(split_arg, data);
		else
			node = create_var_envlist(split_arg, data);
		add_var_envlist(node, data);
		if (split_arg[1])
			rewr[0]++;
}

void	update_var(char **split_arg, t_env *env_var, int *rewr, t_data *data)
{
	env_var->equal = 1;
	if (ft_strcmp(env_var->val, split_arg[2]) != 0)
	{
		if (env_var->val)
			free(env_var->val);
		env_var->val = NULL;
		if (split_arg[2])
			env_var->val = ft_strdup_lim(split_arg[2], '\0', data);
		if (ft_strcmp(env_var->key, "PATH") == 0)
			rewr[1]++;
		rewr[0]++;
	}
}

void	exp_norm(char *arg, int *rewr, t_data *data)
{
	t_env	*env_var;
	t_env	*node;
	char	**split_arg;

	split_arg = split_var_envlist(arg, data, NO);
	if (ft_strncmp(split_arg[0], "_=", 2) != 0)
	{
		env_var = find_var_envlist(split_arg[0], data);
		if (!env_var) // if no var found
			add_new_var(split_arg, rewr, data);
		else if (env_var && split_arg[1]) // if var found and there is an equal sign
			update_var(split_arg, env_var, rewr, data);
	}
	free_strlist(split_arg);
}

void	exp_error(char *arg, int *exit_val, t_data *data)
{
	put_strs_fd(3, data, 2, "TinyShell: export: '", \
	arg,"': not a valid identifier\n");
	*exit_val = 1;
}

int	ft_export(t_cmd *cmd, t_data *data) 
{
	int		i;
	int		*rewr;
	int		status;
	int		exit_val;

	exit_val = 0;
	rewr = ts_calloc(2, sizeof(int), data);
	if (!cmd->array_arg[1])
		return (print_export(data), 0);
	i = 0;
	while (cmd->array_arg[++i])
	{
		status = check_exp_entry(cmd->array_arg[i]);
		if (status == 2)
			exp_error(cmd->array_arg[i], &exit_val, data);
		else if (status == 1)
			exp_concat(cmd->array_arg[i], rewr, data);
		else if (status == 0)
			exp_norm(cmd->array_arg[i], rewr, data);
	}
	if (rewr[0] > 0)
		rewrite_ourenv(data);
	if (rewr[1] > 0)
		get_env_paths(data);
	return (free(rewr), exit_val);
}