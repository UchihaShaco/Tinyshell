/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 18:06:53 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/24 18:14:42 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ret_str;

	if (!s1)
		return (NULL);
	ret_str = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!ret_str)
		return (NULL);
	ft_strlcpy(ret_str, s1, ft_strlen(s1) + 1);
	return (ret_str);
}
