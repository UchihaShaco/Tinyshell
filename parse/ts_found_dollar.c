/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_found_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 20:48:43 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/03 10:22:51 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ts_put_num_error(int err, char **str, int *start)
{
	int		i;
	char	*str_err;

	i = *start;
	str_err = ft_itoa(err);
	ts_replace_key_to_value(str, 2, str_err, i);
	ts_free_str(&str_err);
	(*start)++;
}

int	 ts_found_env_variable(t_data *data, t_cmd *cmd)
{
	int	y;
	int	i;

	i = 0;
	y = 0;
	while (y < cmd->num_arg)
	{
		cmd->arg[y].empty_key = NO;
		while (cmd->arg[y].str[i] != '\0')
		{
			ts_found_dollar(data, &cmd->arg[y].str, cmd->arg[y].q_m, &i);
		}
		if (ft_strlen(cmd->arg[y].str) == 0)
			cmd->arg[y].empty_key = YES;
		y++;
		i = 0;
	}
	return (0);
}

void	ts_found_dollar(t_data *data, char **str, int q_m, int *i_orig)
{
	int	i;

	i = (*i_orig);
	if (i == 0 && (*str)[i] == '~' && (*str)[i + 1] == '\0'
		&& q_m != 39 && q_m != 34)
	{
		ts_free_str(str);
		(*str) = ft_strdup(data->home_dir);
		i = ft_strlen(data->home_dir) - 1;
	}
	if ((*str)[i] == '$' && (*str)[i + 1] == '$')
		i += 2;
	else if ((*str)[i] == '$' && (*str)[i + 1] == '?')
		ts_put_num_error(data->num_prev_error, str, &i);
	else if ((*str)[i] == '$' && (*str)[i + 1] != ' '
		&& ((*str)[i + 1] != '\0' && q_m != 39))
	{
		ts_record_value(data, str, i);
		if ((*str)[0] == '\0')
			return ;
	}
	else
		i++;
	(*i_orig) = i;
}

void	ts_found_dollar_in_name_file(t_data *data, char **file)
{
	int		i;
	char	qm;

	i = 0;
	qm = 0;
	int flag;
	flag = 0;
	while ((*file)[i] != '\0')
	{
		if ((*file)[i] == ONE_Q_MARK && qm != ONE_Q_MARK)
			qm = ONE_Q_MARK;
		else if ((*file)[i] == ONE_Q_MARK && qm == ONE_Q_MARK)
			qm = 0;
		if (qm != ONE_Q_MARK)
		{
			// if (flag > 100) // hard coding this case (minishell$> thisishell $> $G)
			// {
			// i++;
			// flag = 0;
			// }
			data->name_file = YES;
			ts_found_dollar(data, file, qm, &i);
			data->name_file = NO;
		}
		else
			i++;
		// flag++;
	}
}
