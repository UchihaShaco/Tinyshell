#include "../minishell.h"
/* NOTES
	when exporting, variables will only be uploaded to env if there is an equal sign
	if no =, will ONLY show up in export
	export =a
	export /a
	export /=a
	export 1f=a
	export f1=a
	export _=a
	export '=a	
*/
/* print export */
void	print_export(t_data *data)
{
	t_env	*cur;
	t_env	*to_print;

	while (1)
	{
		cur = *(data->env_list);
		while (cur && cur->p == 1)
			cur = cur->next;
		if (cur == NULL)
			break ;
		to_print = cur;
		cur = cur->next;
		while (cur != NULL)
		{
			if (ft_strcmp(to_print->key, cur->key) > 0 && cur->p == 0)
				to_print = cur;
			cur = cur->next;
		}
		to_print->p = 1;
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
	cur = *(data->env_list);
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, "_") != 0)
			cur->p = 0;
		cur = cur->next;
	}
}

/* return 1 if invalid, 0 if not */
int	invalid_export(char *str)
{
	//first char must be alpha or _
	//second char must be alpha, num, _, =, or null
	int	i;
	
	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (1);
	while (str[++i])
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (1);
	return (0);
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

int	ft_export(t_cmd *cmd, t_data *data) 
{	
	t_env	*env_var;
	t_env	*node;
	char	**split_arg;
	int		i;
	int		rewr_ourenv;
	int		rewr_envpaths;
	int		exit_val;
	char	**arg;

	arg = cmd->array_arg;
	if (!arg[1])
	{
		print_export(data);
		return (0);
	}
	i = 0;
	rewr_ourenv = 0;
	rewr_envpaths = 0;
	while (arg[++i])
	{
		split_arg = split_var_envlist(arg[i], data);
		if (invalid_export(split_arg[0]))
		{
			put_strs_fd(3, data, 2, "bash: export: '", arg[i],"': not a valid identifier\n");
			free_strlist(split_arg);
			exit_val = 1;
		}
		else
		{
			if (ft_strncmp(arg[i], "_=", 2) != 0)
			{
				env_var = find_var_envlist(split_arg[0], data);
				//if var doesn't exist
				if (!env_var)
				{
					/* OLDPWD and PWD doesn't exist, and no = is set in the export command */
					if (!split_arg[1] && (ft_strcmp(split_arg[0], "OLDPWD") == 0 || ft_strcmp(split_arg[0], "PWD") == 0))
						node = create_pwd_envlist(split_arg, data);
					else
						node = create_var_envlist(split_arg, data);
					add_var_envlist(node, data);
				}
				if ((!env_var && split_arg[1]))
					rewr_ourenv++;
				// if var exists and there is an = sign in first index (otherwise it would be null)
				else if (env_var && split_arg[1]) 
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
							rewr_envpaths++;
						rewr_ourenv++;
					}
				}
			}
			free_strlist(split_arg);
			exit_val = 0;
		}
	}
	if (rewr_ourenv > 0)
		rewrite_ourenv(data);
	if (rewr_envpaths > 0)
		get_env_paths(data);
	return (exit_val);
}