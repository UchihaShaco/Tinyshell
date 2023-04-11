/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 12:26:41 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/23 14:50:29 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	int	i;

	i = len - 1;
	if (dst == NULL && src == NULL)
		return (NULL);
	if (&((unsigned char *)src)[0] > &((unsigned char *)dst)[0])
		return (ft_memcpy(dst, src, len));
	while (i >= 0)
	{
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		i--;
	}
	return (dst);
}
