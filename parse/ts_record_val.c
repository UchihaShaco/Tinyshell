/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_record_val.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 03:31:14 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 08:53:58 by hbui-vu          ###   ########.fr       */
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

/* start is the index of the first character of 
the key
we use while(s < start ) to get to the start
of the key to give counter t the index 
if the key is empty or the value is empty it will be replaced with 
an empty string it can go to below 0 if the key is longer than the value
*/

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

int	ts_record_key(char *s, int i, char **key, int *digit_key)
{
	int	n;

	n = 0;
	if (ft_isdigit(s[i]) == 1)
	{
		(*key) = "1\0";
		(*digit_key) = YES;
		return (1);
	}
	while (s[i] != '\0' && s[i] != ' ' && s[i] != 34
		&& s[i] != '$' && s[i] != ONE_Q_MARK && s[i] != '=')
	{
		i++;
		n++;
	}
	ts_malloc_str(key, n);
	i = (i - n);
	n = 0;
	while (s[i] != '\0' && s[i] != ' ' && s[i] != 34
		&& s[i] != '$' && s[i] != ONE_Q_MARK && s[i] != '=')
		ts_record_char(key, s, &n, &i);
	(*key)[n] = '\0';
	return (n);
}

int	ts_search_var(t_data *data, char **value, char *key)
{
	int	y;
	int	size_key;
	int	start;

	y = 0;
	size_key = ft_strlen(key);
	start = ft_strlen(key) + 1;
	while (y < data->num_env)
	{
		if (ft_strncmp(key, data->our_env[y], size_key) == 0
			&& (data->our_env[y][size_key] == '='
			|| data->our_env[y][size_key] == '\0'))
		{
			(*value) = ft_strdup_start(data->our_env[y], start);
			return (0);
		}
		y++;
	}
	return (-1);
}

/*value is the value of the key
 i++ to skip the $
 first call of ts_replace_key_to_value 
 is to replace str with empty string NULL
 then second call str is replaced with the value
 we put i - 1 for the start of the key 
 because we want to replace the key with the value
*/
int	ts_record_value(t_data *data, char **str, int i)
{
	int		n;
	char	*key;
	char	*value;
	int		digit_key;

	i++;
	n = 1;
	digit_key = NO;
	value = NULL;
	n = ts_record_key(*str, i, &key, &digit_key);
	if (digit_key == NO)
	{
		ts_search_var(data, &value, key);
		ts_free_str(&key);
	}
	if (value == NULL && data->name_file == YES)
		return (i);
	ts_replace_key_to_value(str, 1, NULL, (i - 1));
	ts_replace_key_to_value(str, n, value, (i - 1));
	ts_free_str(&value);
	return (i);
}
