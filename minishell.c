/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:56 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/29 16:47:41 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ts_err_argc_argv(int argc, char **argv, char **env)
{
	if (argc != 1 || argv == NULL || env == NULL)
	{
		ft_putstr_fd("Tinyshell: this programm complies without arguments\n", 2);
		exit(127);
	}
}

/*Basically this function to return an error if theres an odd number of quotation marks */
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

int	ts_parse(t_data *data, char *line)
{
	int	i;

	i = 0;
	ts_quote_checker(data, line);
	ts_count_and_record_cmd(data, line);
	if (data->num_error != 0 || data->empty_str == YES)
		return (-1);
	while (i < data->num_cmd)
	{
		if (data->num_error == 0)
			ts_found_redirect(&data->cmd[i], data);
		if (data->num_error == 0 && data->cmd[i].str != NULL)
			ts_count_arg_divided_qm(&data->cmd[i], data);
		if (data->num_error == 0 && data->cmd[i].str != NULL)
			ts_create_struct_without_qm(&data->cmd[i], data);
		if (data->num_error == 0 && data->cmd[i].str != NULL)
			ts_found_env_variable(data, &data->cmd[i]);
		i++;
	}
	i = 0; // why do you need this?
	create_fd_pid_array(data);
	return (0);
}

void	ts_init_data(t_data *data, char ***env, int first)
{
	if (first == YES)
	{
		data->cur_dir = getcwd(NULL, 0);
		ts_init_env(data, env);
		init_envlist(data, *env);
		get_env_paths(data);
		// data->name_file = NO; // flag to check if it's a file (YES, NO)
	}
	data->num_prev_error = data->num_error;
	data->num_error = 0;
	data->empty_str = NO; // flag to check if the string is empty (YES, NO)
	data->num_cmd = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*line;

	ts_err_argc_argv(argc, argv, env);
	ft_bzero(&data, sizeof(t_data));
	ts_init_data(&data, &env, YES);
	line = NULL;
	// print_tdata(&data);
	// int def_in = dup(STDIN_FILENO);
	// int def_out = dup(STDOUT_FILENO);
	while (1)
	{
	// 	// dup2(def_in, STDIN_FILENO);
	// 	// dup2(def_out, STDOUT_FILENO);
		ts_get_signal();
		ts_init_data(&data, &env, NO);
		line = readline("\033[1;35mTinyShell > \033[0m");
		ts_signal_ctrl_d(&data, &line);
		ts_parse(&data, line);
		// print_cmds(&data);
		add_history(line);
		if (data.empty_str == NO)
		{
			ts_record_array(&data);
			// print_tdata(&data);
			// print_cmds(&data);
			if (data.num_cmd > 0)
			{
				// for(int i = 0; i < data.num_cmd; i++)
				// 	print_tcmd(&data.cmd[i], i);
				// printf("\n ---------------------------------------\n");
				finalize_cmd(&data);
				// print_cmds(&data);
				// print_tdata(&data);
				execute(&data);
				// for(int i = 0; i < data.num_cmd; i++)
				// 	print_tcmd(&data.cmd[i], i);
			}
		}
		free_data(&data, line, NO);
	}
	free_data(&data, line, YES);
	// ts_free_all(&data, &line); // we will have to free the memory something like this 

}

