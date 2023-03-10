/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 19:34:01 by jalwahei          #+#    #+#             */
/*   Updated: 2023/01/29 13:35:09 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*Copies all the data from the source to the destination*/

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*ptrdst;
	const unsigned char	*ptrsrc;
	size_t				i;

	ptrsrc = src;
	ptrdst = dst;
	i = 0;
	if (dst == NULL && src == NULL )
		return (NULL);
	while (i < n)
	{
		ptrdst[i] = ptrsrc[i];
		i++;
	}
	return (dst);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (!dst && !src)
		return (NULL);
	if (dst < src)
		ft_memcpy(dst, src, len);
	else
	{
		while (len--)
			((char *)dst)[len] = ((char *)src)[len];
	}
	return (dst);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned const char		*alt1;
	unsigned const char		*alt2;

	alt1 = (unsigned const char *)s1;
	alt2 = (unsigned const char *)s2;
	while (n)
	{
		if (*alt1 != *alt2)
			return (*alt1 - *alt2);
		alt1++;
		alt2++;
		n--;
	}
	return (0);
}

void	*ft_memchr(const void *b, int c, size_t n)
{
	size_t			i;
	unsigned char	*s;
	unsigned char	ch;

	ch = c;
	s = (unsigned char *)b;
	i = 0;
	while (i < n)
	{
		if (*s == ch)
		{
			return ((char *)s);
		}
		else
			s++;
	i++;
	}
	return (NULL);
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*altb;

	i = 0;
	altb = (unsigned char *)b;
	while (len > i)
	{
			altb[i] = c;
			i++;
	}
	return (b);
}
