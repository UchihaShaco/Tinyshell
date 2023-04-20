/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:26:35 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/27 15:08:01 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;

	i = 0;
	j = 0;
	if (!dstsize)
		return (ft_strlen(src));
	dst_len = ft_strlen(dst);
	if (dst_len < dstsize && dstsize > 0)
	{
		while (dst[i])
			i++;
		while (i < dstsize - 1 && src[j])
			dst[i++] = src[j++];
		dst[i] = '\0';
	}
	else
		dst_len = dstsize;
	return (dst_len + ft_strlen(src));
}
