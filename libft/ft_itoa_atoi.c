/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 07:01:26 by jalwahei          #+#    #+#             */
/*   Updated: 2023/01/29 13:59:14 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_num(int nbr)
{
	if (nbr < 0)
		return (-nbr);
	else
		return (nbr);
}

static size_t	num_len(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	int		sign;
	char	*c;

	if (n < 0)
	sign = -1;
	else
	sign = 1;
	len = num_len(n);
	c = (char *)malloc(sizeof(char) * len + 1);
	if (!c)
		return (0);
	c[len--] = '\0';
	while (len >= 0)
	{
		c[len] = '0' + ft_num(n % 10);
		n = ft_num(n / 10);
		len--;
	}
	if (sign == -1)
		c[0] = '-';
	return (c);
}

static int	two_case(const char *str)
{
	if (ft_isdigit(str[0]) == 1)
		return (-1);
	if ((ft_strlen(str) == 21) && \
	((str[0] == '-') && ft_isdigit(str[1] == 0)))
		return (0);
	return (0);
}

int	ft_atoi(const char *str)
{
	int					sign;
	long long			res;
	long long			oldres;
	unsigned long long	i;

	i = 0;
	res = 0;
	sign = 1;
	if ((ft_strlen(str) == 20 && str[0] != '-' && \
	ft_isdigit(str[0]) == 1) || ft_strlen(str) == 21)
		return (two_case(str));
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = 1 - 2 * (str[i++] == '-');
	while (str[i] >= '0' && str[i] <= '9')
	{
		oldres = res;
		res = 10 * res + (str[i++] - '0');
		if (oldres > res)
			return (-(sign == 1));
	}
	return ((long long)res * sign);
}
