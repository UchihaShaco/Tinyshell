/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 07:17:27 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 08:58:36 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	as_dir_utils(t_cmd *cmd, t_data *data, struct stat file_stat)
{
	{
		if (!(file_stat.st_mode & S_IXUSR))
		{
			put_strs_fd(3, data, 2, "bash: ", \
			cmd->array_arg[0], ": Permission denied\n");
			data->num_prev_error = 126;
			exit (126);
		}
		if (execve(cmd->array_arg[0], cmd->array_arg, data->our_env) == -1)
		{
			put_strs_fd(3, data, 2, "bash: ", \
			cmd->array_arg[0], ": No such file or directory\n");
			data->num_prev_error = 127;
			exit(127);
		}
	}
}

void	check_as_dir(t_cmd *cmd, int proc, t_data *data)
{
	struct stat	file_stat;
	int			stat_res;

	if (!find_path_separator(cmd) && data->env_paths)
		return ;
	stat_res = stat(cmd->array_arg[0], &file_stat);
	if (stat_res == -1)
	{
		put_strs_fd(3, data, 2, "bash: ", \
		cmd->array_arg[0], ": No such file or directory\n");
		exit(127);
	}
	else if (S_ISDIR(file_stat.st_mode))
	{
		put_strs_fd(3, data, 2, "bash: ", \
		cmd->array_arg[0], ": Is a directory\n");
		exit (126);
	}
	else if (S_ISREG(file_stat.st_mode))
		as_dir_utils(cmd, data, file_stat);
}

int	check_permissions_executable(t_cmd *cmd, t_data *data)
{
	struct stat	file_stat;
	int			stat_res;
	int			permission;

	stat_res = stat(cmd->array_arg[0], &file_stat);
	if (stat_res != -1 && (!(file_stat.st_mode & S_IXUSR)))
	{
		put_strs_fd(3, data, 2, "bash: ", \
		cmd->array_arg[0], ": Permission denied\n");
		exit(126);
	}
	return (0);
}

void	chk_cmd_utils(t_cmd *cmd, t_data *data, struct stat file_stat)
{
	if (cmd->path == NULL)
	{
		if (access(cmd->array_arg[0], F_OK) || S_ISDIR(file_stat.st_mode))
		{
			put_strs_fd(3, data, 2, "bash: ", \
			cmd->array_arg[0], ": command not found\n");
			exit (127);
		}
		else if (access(cmd->array_arg[0], X_OK))
		{
			put_strs_fd(3, data, 2, "bash: ", \
			cmd->array_arg[0], ": Permission denied\n");
			exit(126);
		}
	}
}

void	check_as_command(t_cmd *cmd, int proc, t_data *data)
{
	struct stat	file_stat;
	int			stat_res;

	stat_res = stat(cmd->array_arg[0], &file_stat);
	get_cmd_path(cmd, data);
	chk_cmd_utils(cmd, data, file_stat);
	if (execve(cmd->path, cmd->array_arg, data->our_env) == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(127);
	}
}
