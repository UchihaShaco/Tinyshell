/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 17:03:47 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/27 15:30:59 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	unsigned long long	i;
	int					neg;

	i = 0;
	neg = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			neg *= -1;
	while (ft_isdigit(*str))
	{
		if (i > LLONG_MAX && neg == -1)
			return (0);
		else if (i > LLONG_MAX && neg == 1)
			return (-1);
		i = (i * 10) + (*(str++) - 48);
	}
	return ((int)i * neg);
}
