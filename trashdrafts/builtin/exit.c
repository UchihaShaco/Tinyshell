/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:03:45 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/03 22:03:45 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	str_isnum(char *str)
{
	int	i;

	if (!ft_isdigit(str[0]) && str[0] != '-' && str[0] != '+')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_error(const char *str, char *line, t_data *data)
{
	put_strs_fd(3, data, 2, "TinyShell: exit: ", str, \
	": numeric argument required\n");
	free_data(data, line, YES);
	close(data->defin);
	close(data->defout);
	exit (255);
}

long long	check_exit_val(const char *str, char *line, t_data *data)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			neg *= -1;
	while (str[i])
	{
		if (num > ULLONG_MAX / 10)
		{
			put_strs_fd(3, data, 2, "TinyShell: exit: ", str, ": \
			numeric argument required\n");
			return (255);
		}
		num = (num * 10) + (str[i] - 48);
		i++;
	}
	if ((neg == 1 && num > LLONG_MAX) || (neg == -1 && \
	num > ((unsigned long long)(LLONG_MAX + 1))))
		exit_error(str, line, data);
	return (num * neg);
}

int	ft_exit(t_cmd *cmd, char *line, t_data *data)
{
	int		i;

	if (!cmd->array_arg[1])
	{
		i = 0;
		put_strs_fd(1, data, 1, "exit\n");
	}
	else if (!str_isnum(cmd->array_arg[1]))
	{
		i = 255;
		put_strs_fd(3, data, 2, "TinyShell: exit: ", cmd->array_arg[1], \
		": numeric argument required\n");
	}
	else if (cmd->array_arg[2])
		return (put_strs_fd(1, data, 2, "TinyShell: exit:\
		too many arguments\n"), data->num_prev_error = 1, 1);
	else
	{
		i = check_exit_val(cmd->array_arg[1], line, data);
		put_strs_fd(1, data, 1, "exit\n");
	}
	return (close(data->defin), close(data->defout), \
	free_data(data, line, YES), exit(i), 0);
}
