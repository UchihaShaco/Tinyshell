/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 10:41:25 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/27 15:16:09 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*buffer;
	int		k;

	if (!s1 || !s2)
		return (NULL);
	k = ft_strlen(s1) + ft_strlen(s2) + 1;
	buffer = (char *)malloc(sizeof(char) * k);
	if (!buffer)
		return (NULL);
	ft_strlcpy(buffer, s1, k);
	ft_strlcat(buffer, s2, k);
	return (buffer);
}
