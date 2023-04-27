#include "../minishell.h"
/* NOTES
	unset a=
	unset a=a
	unset =a
	unset a=1
*/

void	ft_unset(t_cmd *cmd, t_data *data)
{
	int		i;
	int		rewrite;
	char	**arg;
	t_env	*env_var;

	i = 1;
	rewrite = 0;
	arg = cmd->array_arg;
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
			{
				if (env_var->equal == 1)
					rewrite++;
				delete_var_envlist(arg[i], data);
			}
			i++;
		}
		if (rewrite > 0)
			rewrite_ourenv(data);
	}
	exit(0);
}



