/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 02:31:21 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 08:20:12 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		error(ERR_CWD, data);
	put_strs_fd(2, data, 1, cwd, "\n");
	free(cwd);
	return (0);
}

void	ts_err_argc_argv(int argc, char **argv, char **env)
{
	if (argc != 1 || argv == NULL || env == NULL)
	{
		ft_putstr_fd("Tinyshell: this programm complies without arguments\n", 2);
		exit(127);
	}
}

void	hqhandle(int sig)
{
	if (sig == SIGINT)
	{
		g_hdsig = 42;
		close(0);
		write(1, "\n", 1);
	}
}

void	qhandler(int sig)
{
	if (sig == SIGQUIT)
	{
	}
}
