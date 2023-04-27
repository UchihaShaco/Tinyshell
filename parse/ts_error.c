/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:42:24 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/27 17:18:08 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ts_err_token(t_data *data, int pipe)
{
	if (pipe == 2)
	{
		data->num_error = ERR_TOKEN;
		data->num_cmd = 0;
		ts_error(data->num_error, "||");
		return (-1);
	}
	data->num_error = ERR_TOKEN;
	data->num_cmd = 0;
	ts_error(data->num_error, "|");
	return (-1);
}

int	ts_error(int error, char *str)
{
	if (error == ERR_TOKEN && (str[0] == 34 || str[0] == 39))
	{
		ft_putstr_fd("Tinyshell: unexpected EOF while ", 2);
		ft_putstr_fd("looking for matching '", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("'\n", 2);
	}
	else if (error == ERR_TOKEN)
	{
		ft_putstr_fd("Tinyshell: syntax error near unexpected token '", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("'\n", 2);
	}
	return (-1);
}

int	ts_error_2(int error, int qm)
{
	char	a;

	a = qm;
	if (error == ERR_TOKEN && qm == 34)
	{
		ft_putstr_fd("Tinyshell: unexpected EOF while ", 2);
		ft_putstr_fd("looking for matching '", 2);
		write(2, &a, 1);
		ft_putstr_fd("'\n", 2);
	}
	if (error == ERR_TOKEN && qm == 39)
	{
		ft_putstr_fd("Tinyshell: unexpected EOF while ", 2);
		ft_putstr_fd("looking for matching '", 2);
		write(2, &a, 1);
		ft_putstr_fd("'\n", 2);
	}
	return (-1);
}
