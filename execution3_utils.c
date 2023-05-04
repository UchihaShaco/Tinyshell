/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 06:42:29 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 06:45:06 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd_array(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->count_redir == 0)
		return ;
	i = -1;
	while (++i < cmd->count_redir)
		if (cmd->fd_array[i] > 0 && close(cmd->fd_array[i]) == -1)
			error(ERR_CLOSE, data);
}

void	close_pipes(t_data *data)
{
	int	j;

	j = 0;
	while (j < data->num_cmd - 1)
	{
		close(data->fd[j][0]);
		close(data->fd[j][1]);
		j++;
	}
}

int	err(char *str, int errno, int proc, t_data *data)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(str, 2);
	strerror(errno);
	close_pipes(data);
	if (proc == CHILD)
		exit(1);
	else
	{
		close(data->defin);
		close(data->defout);
		return (1);
	}
	return (0);
}

int	err_open(t_cmd *cmd, int proc, t_data *data)
{
	ft_putstr_fd(strerror(errno), 2);
	close_fd_array(cmd, data);
	close_pipes(data);
	if (proc == CHILD)
		exit(EXIT_FAILURE);
	else
	{
		close(data->defin);
		close(data->defout);
		return (1);
	}
}

void	redir5(t_cmd *cmd, t_data *data, int i)
{
	if (cmd->last_input == i)
		get_heredoc_fd(cmd, data);
	else
		cmd->fd_array[i] = -2;
}
