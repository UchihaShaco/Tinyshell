#include "../minishell.h"

/*
	if no arguments, go to home
	else, change directories
		print error message if directory doesn't exist and return
	update data->cur_dir
	If oldpwd exists
		update value
	if pwd exists
		update value
	update our_env
*/
int	ft_cd(t_cmd *cmd, t_data *data)
{
	t_env	*pwd;
	t_env 	*oldpwd;
	char	*home;
	char	**arg;
	int		rewrite;

	arg = cmd->array_arg;
	/* if just cd */
	if (!arg[1])
	{
		home = find_home_dir(data);
		if (!home)
		{
			put_strs_fd(1, data, 2, "bash: cd: HOME not set");
			return (1);
		}
		else
			chdir(home);
	}
	else if (chdir(arg[1]) != 0)
	{
		put_strs_fd(3, data, 2, "bash: cd: ", arg[1], " No such file or directory\n");
		return (1);
	}
	if (data->old_dir)
		free(data->old_dir);
	data->old_dir = data->cur_dir;
	data->cur_dir = getcwd(NULL, 0);
	rewrite = 0;
	pwd = find_var_envlist("PWD", data);
	oldpwd = find_var_envlist("OLDPWD", data);
	if (pwd)
	{
		if (pwd->val)
			free(pwd->val);
		pwd->equal = 1;
		pwd->val = ft_strdup_lim(data->cur_dir, '\0', data);
		rewrite++;
	}
	if (oldpwd)
	{
		if (oldpwd->val)
			free(oldpwd->val);
		oldpwd->equal = 1;
		oldpwd->val = ft_strdup_lim(data->old_dir, '\0', data);	
		rewrite++;
	}
	if (rewrite > 0)
		rewrite_ourenv(data);
	return (0);
}