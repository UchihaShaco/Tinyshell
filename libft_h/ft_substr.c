/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 10:22:41 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/26 17:27:43 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*buffer;

	i = 0;
	if (!s)
		return (NULL);
	if (len <= ft_strlen(s))
		buffer = (char *)malloc(sizeof(char) * len + 1);
	else
		buffer = (char *)malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!buffer)
		return (NULL);
	while (i < len && s[start] && start < ft_strlen(s))
		buffer[i++] = s[start++];
	buffer[i] = '\0';
	return (buffer);
}
