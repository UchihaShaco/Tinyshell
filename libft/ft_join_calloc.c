/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_join_calloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 17:52:51 by jalwahei          #+#    #+#             */
/*   Updated: 2023/02/09 13:06:16 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*s;
	size_t		j;
	size_t		k;

	if (!s1 || !s2)
		return (NULL);
	s = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s)
		return (NULL);
	k = 0;
	j = 0;
	while (s1[j] != '\0')
		s[k++] = s1[j++];
	j = 0;
	while (s2[j] != '\0')
		s[k++] = s2[j++];
	s[k] = '\0';
	return (s);
}

char	*ft_strdup(const char *str)
{
	return (ft_substr(str, 0, ft_strlen(str)));
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	strin_len;
	size_t	size;
	char	*sub_str;

	if (!s)
		return (NULL);
	strin_len = ft_strlen(s);
	if (start > strin_len)
		return (ft_strdup(""));
	if (start + len > strin_len)
		len = strin_len - start;
	size = len + 1;
	sub_str = (char *) malloc(size * sizeof(char));
	if (!sub_str)
		return (NULL);
	ft_memcpy(sub_str, s + start, len);
	sub_str[len] = '\0';
	return (sub_str);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	char			*str;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size && count > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
