/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_record_val.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 03:31:14 by jalwahei          #+#    #+#             */
/*   Updated: 2023/03/19 16:52:49 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ts_record_tail(char **tmp, char **str, int t, int start_tail)
{
	int	s;

	s = 0;
	while ((*str)[s] != '\0')
	{
		if (s >= start_tail)
		{
			(*tmp)[t] = (*str)[s];
			t++;
		}
		s++;
	}
	(*tmp)[t] = '\0';
	ts_free_str(str);
	*str = ft_strdup((*tmp));
	ts_free_str(tmp);
}

void	ts_replace_key_to_value(char **str, int key, char *value, int start)
{
	int		s;
	int		t;
	char	*tmp;
	int		size;

	s = 0;
	t = 0;
	size = ft_strlen(*str) + ft_strlen(value) - key;
	if (size <= 0)
	{
		ts_free_str(str);
		ts_malloc_str(str, 0);
		(*str)[0] = '\0';
		return ;
	}
	ts_malloc_str(&tmp, size);
	while (s < start)
		ts_record_char(&tmp, (*str), &t, &s);
	s = 0;
	if (value != NULL)
	{
		while (value[s] != '\0')
			ts_record_char(&tmp, value, &t, &s);
	}
	ts_record_tail(&tmp, str, t, start + key);
}
