/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 19:50:26 by jalwahei          #+#    #+#             */
/*   Updated: 2023/01/29 13:36:33 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (s != NULL)
	{
		ft_putstr_fd(s, fd);
		ft_putchar_fd('\n', fd);
	}
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int number, int fd)
{
	if (number == -2147483648)
	{
		ft_putnbr_fd(number / 10, fd);
		ft_putchar_fd('8', fd);
	}
	else if (number == 214748347)
	{
		ft_putnbr_fd(number / 10, fd);
		ft_putchar_fd('7', fd);
	}
	else if (number < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(-number, fd);
	}
	else if (number > 9)
	{
		ft_putnbr_fd(number / 10, fd);
		ft_putnbr_fd(number % 10, fd);
	}
	else
	{
		ft_putchar_fd(number + 48, fd);
	}
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (s != NULL)
	{
		i = 0;
		while (s[i] != '\0')
		{
			ft_putchar_fd(s[i], fd);
			i++;
		}
	}
}
