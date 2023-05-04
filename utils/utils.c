/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 04:23:11 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 04:24:13 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	else if (!s1 && s2)
		return (0 - s2[i]);
	else if (s1 && !s2)
		return (s1[i] - 0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	return (s1[i] - s2[i]);
}

/* calloc with error management*/

void	*ts_calloc(size_t count, size_t size, t_data *data)
{
	void	*buf;

	buf = ft_calloc(count, size);
	if (!buf)
		error(ERR_MALLOC, data);
	return (buf);
}

/* dup2 with error management */
void	ts_dup2(int oldfd, int newfd, t_data *data)
{
	if (dup2(oldfd, newfd) == -1)
		error(ERR_DUP, data);
}

/* duplicate string up until but not including char c */
char	*ft_strdup_lim(const char *s1, char c, t_data *data)
{
	char	*ret_str;
	int		len;

	if (!s1)
		return (NULL);
	len = 0;
	while (s1[len] != c)
		len++;
	ret_str = (char *) ts_calloc(len + 1, sizeof(char), data);
	ft_strlcpy(ret_str, s1, len + 1);
	return (ret_str);
}

char	*ft_strjoin_char(char const *s1, char const *s2, char c, t_data *data)
{
	char	*buffer;
	int		len;
	int		i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)ft_calloc(len, sizeof(char));
	if (!buffer)
		error(ERR_MALLOC, data);
	i = ft_strlcpy(buffer, s1, len);
	buffer[i] = c;
	if (s2)
		ft_strlcat(buffer, s2, len);
	return (buffer);
}
