/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:56 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/14 15:51:03 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* error if more than one argument passed to program */
static void	ts_err_argc_argv(int argc, char **argv, char **env)
{
	if (argc != 1 || argv == NULL || env == NULL)
	{
		ft_putstr_fd("Tinyshell: this programm complies without arguments\n", 2);
		exit(127);
	}
}

/* what is this for? */
static int ts_quote_checker(t_data *data, char *line)
{
	int i = 0;
	int qm_o = 1;
	int qm_d = 1;
	// printf("line = %s\n ", line);
	while(line[i] != '\0')
	{
		ts_switch_qm(line[i], &qm_o, &qm_d);
		// printf(">>> %c <<< ( single %d) ------ (double %d)",line[i] , qm_o, qm_d);
		i++;
	}
	if (qm_o == -1 || qm_d == -1)
	{
		data->num_error = ERR_TOKEN;
		if (qm_o == -1)
			return (ts_error_2(data->num_error, ONE_Q_MARK));
		return (ts_error_2(data->num_error, DOUBLE_Q_MARK));
	}
	return (0);
}

/* parse single line? */
int	ts_parse(t_data *data, char *line)
{
	int	i;

	i = 0;
	ts_quote_checker(data, line);
	ts_count_and_record_cmd(data, line);
	init_fd_pid(data);
	if (data->num_error != 0 || data->empty_str == YES)
		return (-1);
	
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
	i = 0;
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*line;

	ts_err_argc_argv(argc, argv, env);
	ts_init_data(&data, &env, YES);
	while (1)
	{
		ts_get_signal();
		ts_init_data(&data, &env, NO); 
		line = readline("\033[1;35mTinyShell > \033[0m");
		ts_signal_ctrl_d(&data, &line);
		// ts_signal_ctrl_slash(&data, &line);
		ts_parse(&data, line); 
		add_history(line);
		if (data.empty_str == NO)
		{
			ts_record_array(&data);
			print_t_cmd(data.cmd);
			pipex(&data);
			// ts_free_cycle(&data, &line); // we will have to free the memory something like this 
		}
	}
}


// ts_found_dollar_sign(t_data *data, char *str)
// {
// 	int i = 0;
// 	int j = 0;

// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '$')
// 		{
// 			j = i;
// 			while (str[j] != '\0')
// 			{
// 				if (str[j] == ' ')
// 					break;
// 				j++;
// 			}
// 			ts_found_env_variable(data, str, i, j);
// 		}
// 		i++;
// 	}
// }
