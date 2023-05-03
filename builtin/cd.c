/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 21:52:04 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/03 21:52:04 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_pwd(t_cmd *cmd, t_data *data)
{
	t_env	*pwd;
	t_env	*oldpwd;
	int		rewrite;

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
}

void	update_directory(t_data *data)
{
	if (data->old_dir)
		free(data->old_dir);
	data->old_dir = data->cur_dir;
	data->cur_dir = getcwd(NULL, 0);
}

int	cd_home(t_data *data)
{
	char	*home;

	home = find_home_dir(data);
	if (!home)
	{
		put_strs_fd(1, data, 2, "TinyShell: cd: HOME not set\n");
		return (1);
	}
	else if (chdir(home) == -1)
	{
		put_strs_fd(3, data, 2, "TinyShell: cd: ", home, \
		" No such file or directory\n");
		return (1);
	}
	return (0);
}

int	ft_cd(t_cmd *cmd, t_data *data)
{
	char	**arg;

	arg = cmd->array_arg;
	if (!arg[1])
	{
		if (cd_home(data) == 1)
			return (1);
	}
	else if (chdir(arg[1]) != 0)
	{
		put_strs_fd(3, data, 2, "TinyShell: cd: ", arg[1], \
		" No such file or directory\n");
		return (1);
	}
	update_directory(data);
	change_pwd(cmd, data);
	return (0);
}
