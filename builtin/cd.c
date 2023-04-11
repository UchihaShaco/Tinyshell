#include "../minishell.h"
//change directory display??????

void	ft_cd(char **arg, t_data *data)
{
	char	*curr_dir;
	char	*old_dir;
	t_env	*pwd_var;
	t_env 	*old_pwd_var;

	//if no arguments -> go to home
	if (arg[1] == NULL)
		chdir(data->home_dir);
	//change directories
	else 
	{
		//execute chdir and if it errors out that write error code
		if (chdir(arg[1]) != 0)
		{
			printf("-bash: cd: %s No such file or directory\n", arg[1]);
			error(data);
		}
	}
	//copy over previous directory
	pwd_var = find_var_envlist("PWD", data);
	old_dir = ft_strdup_lim(pwd_var->val, '\0', data);
	//find oldpwd if it exists
	old_pwd_var = find_var_envlist("OLDPWD", data);
	//if oldpwd doesn't exist, add env; else, free old value and create new value. also modify in our_env
	if (old_pwd_var == NULL)
		add_env_var("OLDPWD", old_dir, data);
	else
	{
		free(old_pwd_var->val);
		old_pwd_var->val = old_dir;
		modify_our_env(old_pwd_var, data);
	}
	//change pwd var
	curr_dir = getcwd(NULL, 0);
	pwd_var->val = curr_dir;
	modify_our_env(pwd_var, data);
}