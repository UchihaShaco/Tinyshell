/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_measure_size_file_name.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 08:04:18 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 01:56:15 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ts_err_name_file(t_data *data, int qm_o, int qm_d)
{
	char	qmd[2];

	qmd[0] = DOUBLE_Q_MARK;
	qmd[1] = '\0';
	data->num_error = ERR_TOKEN;
	if (qm_o != 1)
		ts_error(ERR_TOKEN, "'");
	else if (qm_d != 1)
		ts_error(ERR_TOKEN, qmd);
	return (-1);
}

int	ts_measure_size_file_name(t_data *d, char *str, int *i)
{
	int	qm_o;
	int	qm_d;
	int	size_str;

	size_str = 0;
	qm_o = 1;
	qm_d = 1;
	while (str[(*i)] != '\0')
	{
		ts_switch_qm(str[(*i)], &qm_o, &qm_d);
		if (str[(*i)] == ' ' && qm_o == 1 && qm_d == 1)
			break ;
		size_str++;
		(*i)++;
	}
	if (qm_o != 1 || qm_d != 1)
		return (ts_err_name_file(d, qm_o, qm_d));
	return (size_str);
}

int	ts_check_quotation_marks(t_cmd *cmd, int i, t_data *data)
{
	char	q_m;

	q_m = cmd->str[i];
	i++;
	while (cmd->str[i] != q_m && cmd->str[i] != '\0')
		i++;
	if (cmd->str[i] == q_m)
		cmd->num_arg++;
	else
	{
		data->num_error = ERR_TOKEN;
		cmd->num_arg = 0;
		cmd->array_empty = YES;
		return (ts_error_2(data->num_error, q_m));
	}
	return (i + 1);
}

void	ts_check_empty_str(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->str[i] == '\0')
		ts_free_str(&cmd->str);
	else if (cmd->str[i] == ' ')
	{
		while (cmd->str[i] == ' ' && cmd->str[i] != '\0')
			i++;
		if (cmd->str[i] == '\0')
			ts_free_str(&cmd->str);
	}
}
