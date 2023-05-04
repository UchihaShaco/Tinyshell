/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 04:37:24 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 04:38:20 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* prints strings and accounts for printf errors */
void	print_string(int num_str, ...)
{
	int		i;
	t_data	*data;
	va_list	str;

	va_start(str, num_str);
	i = 0;
	data = va_arg(str, t_data *);
	while (i < num_str)
	{
		if (printf("%s", va_arg(str, char *)) == -1)
			error(ERR_PRINT, data);
		i++;
	}
	va_end(str);
}

int	putstr_fd(char *s, int fd)
{
	if (!s)
		return (0);
	while (*s)
		if (write(fd, s++, 1) == -1)
			return (-1);
	return (0);
}
/* prints strings and accounts for printf errors */

void	put_strs_fd(int num_str, ...)
{
	int		i;
	int		fd;
	t_data	*data;
	va_list	str;

	va_start(str, num_str);
	i = -1;
	data = va_arg(str, t_data *);
	fd = va_arg(str, int);
	while (++i < num_str)
	{
		if (putstr_fd(va_arg(str, char *), fd) == -1)
			error(ERR_PRINT, data);
	}
	va_end(str);
}
