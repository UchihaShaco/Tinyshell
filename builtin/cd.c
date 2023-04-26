#include "../minishell.h"
/*
	if no arguments, go to home
	else, change directories
		print error message if directory doesn't exist and return
	update data->cur_dir
	update data->old_dir
	If oldpwd exists
		update value
	if pwd exists
		update value
	update our_env
*/
void	ft_cd(char **arg, t_data *data)
{
	t_env	*pwd;
	t_env 	*oldpwd;

	if (arg[1] == NULL)
		chdir(find_home_dir(data));
	else 
	{
		if (chdir(arg[1]) != 0)
		{
			printf("-bash: cd: %s No such file or directory\n", arg[1]);
			return ;
		}
	}
	if (data->old_dir)
		free(data->old_dir);
	data->old_dir = data->cur_dir;
	data->cur_dir = getcwd(NULL, 0);
	pwd = find_var_envlist("PWD", data);
	if (pwd)
	{
		free(pwd->val);
		pwd->val = ft_strdup_lim(data->cur_dir, '\0', data);
		modify_our_env(pwd, data);
	}
	oldpwd = find_var_envlist("OLDPWD", data);
	if (oldpwd)
	{
		free(oldpwd->val);
		oldpwd->val = ft_strdup_lim(data->old_dir, '\0', data);
		modify_our_env(oldpwd, data);
	}
}