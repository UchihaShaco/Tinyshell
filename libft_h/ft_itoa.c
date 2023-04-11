/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 10:07:18 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/27 15:43:41 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	num_len(int n)
{
	int	count;

	if (n <= 0 || n == 0)
		count = 1;
	else
		count = 0;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{	
	long	num;
	char	*nbr;
	int		len;

	nbr = (char *)malloc(sizeof(char) * num_len(n) + 1);
	if (!nbr)
		return (0);
	num = n;
	len = num_len(n);
	nbr[len--] = '\0';
	if (num == 0)
		nbr[0] = '0';
	if (num < 0)
	{
		nbr[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		nbr[len--] = (num % 10) + 48;
		num /= 10;
	}
	return (nbr);
}
