/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_quotation_marks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 18:03:37 by jalwahei          #+#    #+#             */
/*   Updated: 2023/02/27 11:00:29 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ts_switch_qm(char c, int *qm_o, int *qm_d)
{
	if (c == DOUBLE_Q_MARK && (*qm_o) == 1)
		(*qm_d) = (*qm_d) * (-1);
	if (c == ONE_Q_MARK && (*qm_d) == 1)
		(*qm_o) = (*qm_o) * (-1);
}

void	ts_search_space_after_arg(char *str, t_arg *arg, int i)
{
	int	c;

	c = 0;
	if (str[i] == ' ')
	{
		c = i;
		while (str[c] != '\0' && str[c] == ' ')
			c++;
		if (str[c] != '\0')
			arg->space = YES;
	}
}

int	ts_cut_quotation_marks(char *str, t_arg *arg, int i)
{
	int		c;
	char	q_m;

	c = 0;
	q_m = str[i];
	i++;
	arg->q_m = (int)q_m;
	while (str[i] != q_m && str[i] != '\0')
	{
		c++;
		i++;
	}
	ts_malloc_str(&arg->str, c);
	i = i - c;
	c = 0;
	while (str[i] != q_m && str[i] != '\0')
		ts_record_char(&arg->str, str, &c, &i);
	arg->str[c] = '\0';
	if (str[i] == q_m)
	{
		i++;
		ts_search_space_after_arg(str, arg, i);
	}
	return (i);
}

int	ts_record_args_without_qm(char *str, t_arg *arg, int i, int *num_arg)
{
	int	c;

	c = 0;
	arg->q_m = NO;
	if (str[i] != ' ')
	{
		while (str[i] != 39 && str[i] != 34 && str[i] != '\0' && str[i] != ' ')
		{
			c++;
			i++;
		}
		ts_malloc_str(&arg->str, c);
		i = i - c;
		c = 0;
		while (str[i] != 39 && str[i] != 34 && str[i] != '\0' && str[i] != ' ')
			ts_record_char(&arg->str, str, &c, &i);
		arg->str[c] = '\0';
		ts_search_space_after_arg(str, arg, i);
		(*num_arg)++;
		return (i);
	}
	return (i + 1);
}

void	ts_create_struct_without_qm(t_cmd *cmd)
{
	int	i;
	int	num_arg;

	i = 0;
	num_arg = 0;
	ts_malloc_arg(&cmd->arg, cmd->num_arg);
	while (cmd->str[i] != '\0')
	{
		cmd->arg[num_arg].space = NO;
		if (cmd->str[i] == 34 || cmd->str[i] == 39)
		{
			i = ts_cut_quotation_marks(cmd->str, &cmd->arg[num_arg], i);
			num_arg++;
		}
		else
		{
			while (cmd->str[i] != 34 && cmd->str[i] != 39
				&& cmd->str[i] != '\0')
				{i = ts_record_args_without_qm(cmd->str, &cmd->arg[num_arg], i, &num_arg);
				// print_arg(&cmd->arg[num_arg]);
}
		}
	}
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

int	ts_count_args_without_qm(t_cmd *cmd, int i)
{
	if (cmd->str[i] == ' ')
	{
		while (cmd->str[i] == ' ')
			i++;
	}
	if (cmd->str[i] != 39 && cmd->str[i] != 34 && cmd->str[i] != '\0')
		cmd->num_arg++;
	while (cmd->str[i] != 39 && cmd->str[i] != 34 && cmd->str[i] != '\0')
	{
		if (cmd->str[i] == ' ')
		{
			while (cmd->str[i] == ' ' && cmd->str[i] != '\0')
				i++;
			if (cmd->str[i] != 39 && cmd->str[i] != 34 && cmd->str[i] != '\0')
				cmd->num_arg++;
		}
		else
			i++;
	}
	return (i);
}

int	ts_count_arg_divided_qm(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 0;
	cmd->num_arg = 0;
	while (cmd->str[i] != '\0')
	{
		if (cmd->str[i] == 34 || cmd->str[i] == 39)
			i = ts_check_quotation_marks(cmd, i, data);
		else
			i = ts_count_args_without_qm(cmd, i);
		if (i == -1)
			return (-1);
	}

	return (0);
}
// void print_arg(t_arg *arg) {
//     printf("str: %s\n", arg->str);
//     printf("q_m: %d\n", arg->q_m);
//     printf("space: %d\n", arg->space);
//     printf("redir: %d\n", arg->redir);
//     printf("empty_key: %d\n", arg->empty_key);
// }