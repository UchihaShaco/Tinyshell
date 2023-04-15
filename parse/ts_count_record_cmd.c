/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_count_record_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 09:47:34 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/01 04:23:58 by jalwahei         ###   ########.fr       */
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
	int	i;
	int	qm_d;
	int	qm_o;

	i = 0;
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

static int	ts_record_one_str(char **str, char *line, int *start, int *num)
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	size = ts_get_size_one_cmd_str(line, start, size);
	ts_malloc_str(str, size);
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
	// printf("line = %s \n", line);
	while (line[i] != '\0')
	{
		ts_switch_qm(line[i], &qm_o, &qm_d);
		// printf("line[i] = %c \t qm_o = %d \t qm_d = %d \t num_cmd = %d counter = %d\n", line[i], qm_o, qm_d, data->num_cmd, i);
		if (line[i] == '|' && qm_o == 1 && qm_d == 1)
		{
			// printf("line[i] = %c \n", line[i] );
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

int	ts_count_and_record_cmd(t_data *data, char *line)
{
	int	i;
	int	num;
	// int	qm_d;
	// int	qm_o;

	i = 0;
	// qm_o = 1000;
	// qm_d = 1;
	data->num_cmd = 1;
	if (ts_check_empty_and_err_token_pipe(data, line) == -1)
		return (-1);
	if (ts_count_pipe(data, line, 1, 1) == -1)
		return (-1);
	ts_malloc_cmd(&data->cmd, data->num_cmd);
	i = 0;
	while (data->num_cmd > i)
	{
		data->cmd[i] = (t_cmd){};
		// print_t_data(struct s_data data)
		i++;
	}
	i = 0;
	num = 0;
	while (line[i] != '\0')
			ts_record_one_str(&data->cmd[num].str, line, &i, &num);
			// print_t_data(*data);

	return (0);
}
	// if ((line[i - 1] == 34 && qm_d ==  -1 ) || (line[i - 1] == 39 && qm_o == -1))
	// 	{
	// 		data->num_error = ERR_TOKEN;
	// 		return (ts_error_2(data->num_error, line[i - 1]));
	// 	}
// {
//     printf("t_data {\n");
// 	int i = 0;
// 	while(data.cmd[i].str != NULL)
//     	{printf("    t_cmd *cmd: %s\n", data.cmd[i].str);
// 	i++;}
//     printf("    t_tmp tmp: %d\n", data.tmp);
//     printf("    int num_cmd: %d\n", data.num_cmd);
//     printf("    int num_error: %d\n", data.num_error);
//     printf("    int num_prev_error: %d\n", data.num_prev_error);
//     printf("    int num_env: %d\n", data.num_env);
//     printf("    char **our_env: %p\n", data.our_env);
//     printf("    char **tmp_var: %p\n", data.tmp_var);
//     printf("    char *prev_dir: %p\n", data.prev_dir);
//     printf("    char *cur_dir: %s\n", data.cur_dir);
//     printf("    char *home_dir: %s\n", data.home_dir);
//     printf("    int flag_old: %d\n", data.flag_old);
//     printf("    int empty_str: %d\n", data.empty_str);
//     printf("    int fd_pipe[2]: {%d, %d}\n", data.fd_pipe[0], data.fd_pipe[1]);
//     printf("    int name_file: %d\n", data.name_file);
//     printf("    int build_in: %d\n", data.build_in);
//     printf("    int *pid: %p\n", data.pid);
//     printf("    int n_end: %d\n", data.n_end);
//     printf("}\n");
// }