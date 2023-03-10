/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 23:33:19 by jalwahei          #+#    #+#             */
/*   Updated: 2023/01/29 13:38:34 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
DESCRIPTION
 The strnstr() function locates the first occurrence of the null-terminated 
 string needle in the string haystack,
	where not more than len characters are searched.  Characters that appear
     after a `\0' character are not searched.  
*/

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	hay_len;
	size_t	need_len;

	if (*needle == '\0' )
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	hay_len = ft_strlen(haystack);
	need_len = ft_strlen(needle);
	if (hay_len < need_len || len < need_len)
		return (NULL);
	while (len-- >= need_len && *haystack)
	{
		if (ft_memcmp(haystack, needle, need_len) == 0)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*ps1;
	unsigned char	*ps2;

	ps1 = (unsigned char *)s1;
	ps2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (ps1[i] && ps1[i] == ps2[i] && (i < n - 1))
		i++;
	return (ps1[i] - ps2[i]);
}

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	i++;
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (0);
}

static char	ft_trim(char c, char const *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *str1, char const *str2)
{
	char	*str;
	size_t	i;
	size_t	start;
	size_t	end;

	if (!str1 || !str2)
		return (0);
	start = 0;
	while (str1[start] && ft_trim(str1[start], str2))
		start++;
	end = ft_strlen(str1);
	while (end > start && ft_trim(str1[end - 1], str2))
		end--;
	str = (char *)malloc(sizeof(*str1) * (end - start + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (start < end)
		str[i++] = str1[start++];
	str[i] = 0;
	return (str);
}
