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
void	ft_cd(char **arg, t_data *data)
{
	t_env	*pwd;
	t_env 	*oldpwd;
	char	*home;

	if (arg[1] == NULL)
	{
		home = find_home_dir(data);
		if (!home)
			print_string(1, data, "bash: cd: HOME not set");
		else
			chdir(home);
	}
	else 
	{
		if (chdir(arg[1]) != 0)
		{
			printf("-bash: cd: %s No such file or directory\n", arg[1]);
			return ;
		}
	}
}