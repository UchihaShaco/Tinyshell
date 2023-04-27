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
		print_string(2, data, "declare -x ", to_print->key);
		if (to_print->equal == 1)
		{
			print_string(1, data, "=");
			if (to_print->val == NULL)
				print_string(1, data, """\n");
			else
				print_string(2, data, to_print->val, "\n");
		}
		else
			print_string(1, data, "\n");
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

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (1);
	if (!ft_isalnum(str[1]) && str[1] != '_' && str[1] != '=' && str[1] != '\0')
		return (1);
	return (0);
}

void	ft_export(t_cmd *cmd, t_data *data) 
{	
	t_env	*env_var;
	char	**split_arg;
	int		i;
	int		rewr_ourenv;
	int		rewr_envpaths;
	char	**arg;

	arg = cmd->array_arg;
	if (!arg[1])
	{
		print_export(data);
		return ;
	}
	i = 0;
	rewr_ourenv = 0;
	rewr_envpaths = 0;
	while (arg[++i])
	{
		if (invalid_export(arg[i]))
			printf("-bash: export: '%s': not a valid identifier\n", arg[i]);
		else if (ft_strncmp(arg[i], "_=", 2) != 0)
		{
			split_arg = split_var_envlist(arg[i], data);
			env_var = find_var_envlist(split_arg[0], data);
			if (!env_var)
				add_var_envlist(split_arg, data);
			if ((!env_var && split_arg[1]))
				rewr_ourenv++;
			// if var exists and there is an = sign in first index (otherwise it would be null)
			else if (env_var && split_arg[1]) 
			{
				if (ft_strcmp(env_var->val, split_arg[2]) != 0)
				{
					if (ft_strcmp(env_var->key, "PATH") == 0)
						rewr_envpaths++;
					free(env_var->val);
					env_var->val = NULL;
					if (split_arg[2])
						env_var->val = ft_strdup_lim(split_arg[2], '\0', data);
					rewr_ourenv++;
				}
			}
			free_strlist(split_arg);
		}
	}
	if (rewr_ourenv > 0)
		rewrite_ourenv(data);
	exit(0);
}




