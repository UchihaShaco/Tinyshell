/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 12:11:07 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/24 12:23:01 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ret_s;

	i = 0;
	ret_s = (unsigned char *)s;
	while (i < n)
	{
		if (ret_s[i] == (unsigned char)c)
			break ;
		i++;
	}
	if (i == n)
		return (NULL);
	return (ret_s + i);
}
