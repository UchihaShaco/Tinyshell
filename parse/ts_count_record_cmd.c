/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_count_record_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 09:47:34 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/03 14:46:46 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ts_check_empty_and_err_token_pipe(t_data *data, char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
	{
		data->empty_str = YES;
		return (-1);
	}
	if (line[i] == '|')
	{
		data->num_error = ERR_TOKEN;
		data->num_cmd = 0;
		if (line[i + 1] == '|')
			return (ts_error(data->num_error, "||"));
		return (ts_error(data->num_error, "|"));
	}
	i = 0;
	while (line[i] != '\0')
		i++;
	if (line[i - 1] == '|')
	{
		data->num_error = ERR_TOKEN;
		data->num_cmd = 0;
		return (ts_error(data->num_error, "|"));
	}
	return (0);
}

static int	ts_get_size_one_cmd_str(char *line, int *start, int size)
{
	int	qm_d;
	int	qm_o;

	qm_o = 1;
	qm_d = 1;
	while (line[(*start) + size] != '\0')
	{
		ts_switch_qm(line[(*start) + size], &qm_o, &qm_d);
		if (line[(*start) + size] == '|' && qm_o == 1 && qm_d == 1)
			break ;
		size++;
	}
	return (size);
}

static int	ts_record_one_str(char **str, char *line, int *start, int *num, t_data *data)
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	size = ts_get_size_one_cmd_str(line, start, size);
	ts_malloc_str(str, size, data);
	while (i < size)
	{
		(*str)[i] = line[(*start) + i];
		i++;
	}
	(*str)[i] = '\0';
	(*num)++;
	if (line[(*start) + i] == '|')
		i++;
	(*start) = (*start) + i;
	return (0);
}

static int	ts_count_pipe(t_data *data, char *line, int qm_d, int qm_o)
{
	int	i;

	i = 0;

	while (line[i] != '\0')
	{
		ts_switch_qm(line[i], &qm_o, &qm_d);
		if (line[i] == '|' && qm_o == 1 && qm_d == 1)
		{
			i++;
			if (line[i] == '|')
				return (ts_err_token(data, 2));
			if (line[i] == ' ')
			{
				while (line[i] == ' ' && line[i] != '\0')
					i++;
			}
			if (line[i] == '\0' || line[i] == '|')
				return (ts_err_token(data, 1));
			data->num_cmd++;
		}
		else
		i++;
	}
	return (0);
}
// counts the number of commands and records them in the structure each commmand in a separate structure as a string
int	ts_count_and_record_cmd(t_data *data, char *line)
{
	int	i;
	int	num;

	i = 0;
	data->num_cmd = 1;
	if (ts_check_empty_and_err_token_pipe(data, line) == -1)
		return (-1);
	if (ts_count_pipe(data, line, 1, 1) == -1)
		return (-1);
	ts_malloc_cmd(&data->cmd, data->num_cmd, data);
	//can replace with ts_calloc(&data->cmd, data->num_cmd, data);
	i = 0;
	while (data->num_cmd > i)
	{
		data->cmd[i] = (t_cmd){};
		i++;
	}
	i = 0;
	num = 0;
	while (line[i] != '\0')
		ts_record_one_str(&data->cmd[num].str, line, &i, &num, data);
	return (0);
}
