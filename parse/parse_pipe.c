/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 18:57:52 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 01:37:31 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_error_pipe(int error, char *str)
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
	else if (error == ERR_NUM_ONE && str[0] == '!')
	{
		ft_putstr_fd("Tinyshell: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": event not found\n", 2);
	}
	return (-1);
}

int	ts_check_empty_and_err_pipe(t_data *data, char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
	{
		data->empty_str = YES;
		return (-1);
	}
	if (line[i] == '|')
	{
		data->num_error = ERR_TOKEN;
		data->num_cmd = 0;
		if (line[i + 1] == '|')
			return (print_error_pipe(data->num_error, "||"));
		return (print_error_pipe(data->num_error, "|"));
	}
	return (0);
}
