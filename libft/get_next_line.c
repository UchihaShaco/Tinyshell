/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 15:49:15 by jalwahei          #+#    #+#             */
/*   Updated: 2023/01/29 13:25:21 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static char	*ft_strchr(const char *s, int c)
// {
// 	size_t	i;
// 	char	*str;

// 	i = 0;
// 	str = (char *)s;
// 	if (!str[0])
// 		return (NULL);
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == (unsigned char)c)
// 			return (str + i);
// 		i++;
// 	}
// 	if ((unsigned char)c == '\0')
// 		return (str + i);
// 	return (NULL);
// }

static char	*ft_strjoinn(char *s1, char *s2)
{
	size_t	i;
	char	*str;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s1[i] != '\0')
	{
		str[j++] = s1[i++];
	}
	i = 0;
	while (s2[i] != '\0')
	{
		str[j++] = s2[i++];
	}
	str[j] = '\0';
	free(s1);
	return (str);
}

char	*read_line(int fd, char *static_buffer)
{
	int		k;
	char	*buffer;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	k = 1;
	if (!static_buffer)
		static_buffer = ft_strdup("");
	while (!ft_strchr(static_buffer, '\n') && k != 0)
	{
		k = read(fd, buffer, BUFFER_SIZE);
		if (k == -1)
		{
			free(buffer);
			free(static_buffer);
			return (NULL);
		}
		buffer[k] = '\0';
		static_buffer = ft_strjoinn(static_buffer, buffer);
	}
	free(buffer);
	return (static_buffer);
}

char	*ft_getline(char *read)
{
	char	*line;
	int		i;

	i = 0;
	if (!read[i])
		return (NULL);
	while (read[i] && read[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (read[i] && read[i] != '\n')
	{
		line[i] = read[i];
		i++;
	}
	if (read[i] == '\n' || read[i] == '\0')
	{
		line[i] = read[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_free(char *buffer)
{
	int		i;
	int		j;
	char	*saving;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	saving = (char *)malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
	if (!saving)
		return (NULL);
	i++;
	j = 0;
	while (buffer[i])
		saving[j++] = buffer[i++];
	saving[j] = '\0';
	free(buffer);
	return (saving);
}

char	*get_next_line(int fd)
{
	static char		*buffer;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > 2147483646)
		return (NULL);
	buffer = read_line(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_getline(buffer);
	buffer = ft_free(buffer);
	return (line);
}
