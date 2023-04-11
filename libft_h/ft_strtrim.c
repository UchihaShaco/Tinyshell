/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 21:51:43 by hbui-vu           #+#    #+#             */
/*   Updated: 2022/09/27 17:20:20 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_sep(char c, char const *charset)
{
	while (*charset)
		if (c == *(charset++))
			return (1);
	return (0);
}

static int	str_size(char const *s1, char const *set)
{
	int	count;
	int	len;

	count = 0;
	len = ft_strlen(s1) - 1;
	while (s1[count] && is_sep(s1[count], set))
		count++;
	if (count == (int)ft_strlen(s1))
		return (0);
	while (s1[len] && is_sep(s1[len], set))
	{
		len--;
		count++;
	}
	len = ft_strlen(s1) - count + 1;
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		i;
	int		len;

	if (!s1)
		return (NULL);
	len = str_size(s1, set);
	str = (char *)malloc(sizeof(char) * (len));
	if (!str)
		return (NULL);
	i = 0;
	while (*s1 && is_sep(*s1, set))
		s1++;
	while (i < len - 1)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
