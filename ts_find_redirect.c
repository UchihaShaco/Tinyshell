/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_find_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:40:55 by jalwahei          #+#    #+#             */
/*   Updated: 2023/03/10 16:32:44 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ts_init_emum_redir(t_cmd *cmd, int *i_orig, int num_redir)
{
	int	i;

	i = (*i_orig);
	if (cmd->str[i] == '>')
		cmd->redir[num_redir] = REDIR_W;
	else if (cmd->str[i] == '<')
		cmd->redir[num_redir] = REDIR_R;
	if (cmd->str[i] == '>' && cmd->str[i + 1] == '>')
	{
		cmd->redir[num_redir] = REDIR_W_ADD;
		i++;
	}
	else if (cmd->str[i] == '<' && cmd->str[i + 1] == '<')
	{
		cmd->redir[num_redir] = HEREDOC;
		i++;
	}
	else if (cmd->str[i] == '<' && cmd->str[i + 1] == '>')
	{
		cmd->redir[num_redir] = REDIR_RONG;
		i++;
	}
	i++;
	(*i_orig) = i;
}

int	ts_error_parse_redir(t_data *data, char *s, int i)
{
	if (s[i] == '>' || s[i] == '<' || s[i] == '\0')
	{
		data->num_error = ERR_TOKEN;
		if (s[i] == '\0')
			return (ts_error(data->num_error, "newline"));
		if (s[i] == '>')
		{
			if (s[i + 1] == '>')
				return (ts_error(data->num_error, ">>"));
			else
					return (ts_error(data->num_error, ">"));
		}
		if (s[i] == '<')
		{
			if (s[i + 1] == '<')
				return (ts_error(data->num_error, "<<"));
			else if (s[i + 1] == '>')
				return (ts_error(data->num_error, "<>"));
			else
				return (ts_error(data->num_error, "<"));
		}
	}
	return (0);
}

int	ts_count_redirect(t_cmd *cmd, t_data *data, int qm_o, int qm_d)
{
	int	i;

	i = 0;
	cmd->count_redir = 0;
	while (cmd->str[i] != '\0')
	{
		ts_switch_qm(cmd->str[i], &qm_o, &qm_d);
		if ((cmd->str[i] == '>' || cmd->str[i] == '<')
			&& qm_d == 1 && qm_o == 1)
		{
			if ((cmd->str[i] == '>' && cmd->str[i + 1] == '>')
				|| (cmd->str[i + 1] == '<' && cmd->str[i] == '<'))
				i++;
				// || (cmd->str[i] == '<' && cmd->str[i + 1] == '>'))
			i++;
			while (cmd->str[i] == ' ' && cmd->str[i] != '\0')
				i++;
			if (ts_error_parse_redir(data, cmd->str, i) == -1)
				return (-1);
			cmd->count_redir++;
		}
		else
			i++;
	}
	return (cmd->count_redir);
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

int	ts_cycle_of_record_redir(t_cmd *cmd, t_data *data, int qm_o, int qm_d)
{
	int	i;
	int	num_redir;

	i = 0;
	num_redir = 0;
	while (cmd->str[i] != '\0')
	{
		ts_switch_qm(cmd->str[i], &qm_o, &qm_d);
		if ((cmd->str[i] == '>' || cmd->str[i] == '<')
			&& qm_d == 1 && qm_o == 1)
		{
			if (ts_record_redir_and_file(cmd, i, num_redir, data) == -1)
				return (-1);
			ts_replace_key_to_value(&cmd->str, data->tmp.size_cut, NULL, i);
			if (cmd->str[i] == '\0' && i > 0)
				i--;
			num_redir++;
			if (cmd->str[0] == '\0')
				break ;
		}
		i++;
	}
	ts_check_empty_str(cmd);
	return (0);
}

int	 ts_found_redirect(t_cmd *cmd, t_data *data)
{
	int	qm_d;
	int	qm_o;

	qm_o = 1;
	qm_d = 1;
	if (ts_count_redirect(cmd, data, qm_o, qm_d) <= 0)
	{
		cmd->count_redir = 0;
		return (-1);
	}
	ts_malloc_arr_int(&cmd->redir, cmd->count_redir);
	ts_malloc_array(&cmd->file, cmd->count_redir);
	ts_cycle_of_record_redir(cmd, data, qm_o, qm_d);
	cmd->file[cmd->count_redir] = NULL;
	cmd->redir[cmd->count_redir] = 0;
	return (0);
}

int	ts_record_redir_and_file(t_cmd *cmd, int i, int num_redir, t_data *d)
{
	int	start;
	int	size_str;

	start = 0;
	d->tmp.size_cut = i;
	ts_init_emum_redir(cmd, &i, num_redir);
	while (cmd->str[i] == ' ' && cmd->str[i] != '\0')
		i++;
	start = i;
	size_str = ts_measure_size_file_name(d, cmd->str, &i);
	if (size_str == -1)
		return (-1);
	ts_malloc_str(&cmd->file[num_redir], size_str);
	ts_record_str(&cmd->file[num_redir], cmd->str, start, size_str);
	// ts_found_dollar_in_name_file(d, &cmd->file[num_redir]); handle it here
	ts_cut_qm_in_name_file(&cmd->file[num_redir]);
	d->tmp.size_cut = i - d->tmp.size_cut;
	return (0);
}
