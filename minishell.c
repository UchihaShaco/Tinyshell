/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:56 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 11:50:16 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	ts_quote_checker(t_data *data, char *line)
{
	int	i;
	int	qm_o;
	int	qm_d;

	i = 0;
	qm_o = 1;
	qm_d = 1;
	while (line[i] != '\0')
		ts_switch_qm(line[i++], &qm_o, &qm_d);
	if (qm_o == -1 || qm_d == -1)
	{
		data->num_error = ERR_TOKEN;
		if (qm_o == -1)
			return (ts_error_2(data->num_error, ONE_Q_MARK));
		return (ts_error_2(data->num_error, DOUBLE_Q_MARK));
	}
	return (0);
}

void	ts_parse(t_data *data, char *line)
{
	int	i;

	i = 0;
	if (!(*line))
		return ;
	ts_quote_checker(data, line);
	if (data->num_error != 0 || data->empty_str == YES)
		return ;
	if (ts_count_and_record_cmd(data, line) == -1)
		return ;
	while (i < data->num_cmd)
	{
		if (data->num_error == 0)
			ts_found_redirect(&data->cmd[i], data);
		if (data->num_error == 0 && data->cmd[i].str != NULL)
			ts_count_arg_divided_qm(&data->cmd[i], data);
		if (data->num_error == 0 && data->cmd[i].str != NULL)
			ts_create_struct_without_qm(&data->cmd[i]);
		if (data->num_error == 0 && data->cmd[i].str != NULL)
			ts_found_env_variable(data, &data->cmd[i]);
		i++;
	}
	create_fd_pid_array(data);
	return ;
}

void	ts_init_data(t_data *data, char ***env, char **line, int first)
{
	if (first == YES)
	{
		data->cur_dir = getcwd(NULL, 0);
		ts_init_env(data, env);
		init_envlist(data, *env);
		get_env_paths(data);
		data->defin = dup(STDIN_FILENO);
		data->defout = dup(STDOUT_FILENO);
		data->name_file = NO;
		*line = NULL;
	}
	data->num_prev_error = data->num_error;
	data->num_error = 0;
	data->empty_str = NO;
	data->num_cmd = 0;
	g_hdsig = 0;
}

void	rec_finalize(t_data *data, char *line)
{
	if (data->empty_str == NO)
	{
		ts_record_array(data);
		if (data->num_cmd > 0)
		{
			finalize_cmd(data);
			if (g_hdsig == 0)
				execute(line, data);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*line;

	ts_err_argc_argv(argc, argv, env);
	ft_bzero(&data, sizeof(t_data));
	ts_init_data(&data, &env, &line, YES);
	while (1)
	{
		dup2(data.defin, STDIN_FILENO);
		dup2(data.defout, STDOUT_FILENO);
		ts_get_signal();
		ts_init_data(&data, &env, &line, NO);
		line = readline("\033[1;35mTinyShell > \033[0m");
		ts_signal_ctrl_d(&data, &line);
		ts_parse(&data, line);
		if (line)
			add_history(line);
		rec_finalize(&data, line);
		free_data(&data, line, NO);
	}
}
