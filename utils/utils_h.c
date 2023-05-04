/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_h.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 04:24:02 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 04:24:02 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ts_split(char *str, char c, t_data *data)
{
	char	**buffer;

	buffer = ft_split(str, c);
	if (!buffer)
		error(ERR_MALLOC, data);
	return (buffer);
}

/* looks for char and returns index of that char if found. else it returns -1 */
int	detect_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

void	free_err_data(t_data *data)
{
	clear_envlist(data->env_list);
	data->env_list = NULL;
	free_strlist(data->our_env);
	data->our_env = NULL;
	free_strlist(data->env_paths);
	data->env_paths = NULL;
	if (data->cur_dir)
	{
		free(data->cur_dir);
		data->cur_dir = NULL;
	}
	if (data->old_dir)
	{
		free(data->old_dir);
		data->old_dir = NULL;
	}
	free_cmd(data);
	free_fdlist(data);
	if (data->pid)
	{
		free(data->pid);
		data->pid = NULL;
	}
}

/* placeholder error function*/
void	error(int error, t_data *data)
{
	(void)data;
	if (error == ERR_MALLOC)
		write(2, "TinyShell: Allocation memory error\n", 35);
	else if (error == ERR_OPEN)
		write(2, "TinyShell: Open function error\n", 31);
	else if (error == ERR_PIPE)
		write(2, "TinyShell: Pipe function error\n", 31);
	else if (error == ERR_DUP)
		write(2, "TinyShell: Dup2 function error\n", 31);
	else if (error == ERR_FORK)
		write(2, "TinyShell: Fork function error\n", 31);
	else if (error == ERR_CWD)
		write(2, "TinyShell: getcwd function error\n", 33);
	else if (error == ERR_EXEC)
		write(2, "TinyShell: exec function error\n", 31);
	else if (error == ERR_CLOSE)
		write(2, "TinyShell: close function error\n", 32);
	close(data->defin);
	close(data->defout);
	free_err_data(data);
	write(2, "Exiting TinyShell\n", 19);
	exit(data->num_prev_error);
}
