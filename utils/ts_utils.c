/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 03:29:53 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 11:48:54 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ts_record_char(char **result, char *str, int *r, int *s)
{
	(*result)[(*r)] = str[(*s)];
	(*r)++;
	(*s)++;
}

void	ts_record_str(char **file, char *str, int start, int size_str)
{
	int	i;

	i = 0;
	while (i < size_str)
	{
		(*file)[i] = str[start + i];
		i++;
	}
	(*file)[i] = '\0';
}

int	ts_cut_qm_in_name_file(char **file)
{
	int		i;
	char	qm;

	i = 0;
	qm = 0;
	while ((*file)[i] != '\0')
	{
		qm = (*file)[i];
		if (qm == 34 || qm == 39)
		{
			ts_replace_key_to_value(file, 1, NULL, i);
			while ((*file)[i] != qm && (*file)[i] != '\0')
				i++;
			if ((*file)[i] != '\0')
				ts_replace_key_to_value(file, 1, NULL, i);
		}
		else
			i++;
	}
	return (0);
}

void	free_util(t_data *data, char *line)
{
	free_cmd(data);
	free_fdlist(data);
	if (data->pid)
	{
		free(data->pid);
		data->pid = NULL;
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
}

char	*ft_strdup_start(const char *s, int start)
{
	int		i;
	char	*str1;

	if (s == NULL)
		return (NULL);
	if ((size_t)start >= ft_strlen(s) || start < 0)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0')
		i++;
	str1 = (char *)malloc(sizeof(char) * (i + 1));
	if (str1 == NULL)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0')
	{
		str1[i] = s[start + i];
		i++;
	}
	str1[i] = '\0';
	return (str1);
}
