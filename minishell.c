/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:56 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/20 09:45:08 by jalwahei         ###   ########.fr       */
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

void	ts_init_data(t_data *data, char ***env, int first)
{
	if (first == YES)
	{
		data->flag_old = 1;
		data->prev_dir = NULL;
		data->cur_dir = getcwd(NULL, 0);
		data->num_prev_error = 0; // check header :P
		data->num_error = 0;
		data->num_tmp_var = 0; // just a tmp var might be useful
		data->tmp_var = NULL;
		ts_init_env(data, env);
		data->name_file = NO; // flag to check if it's a file (YES, NO)
	}
	data->num_prev_error = data->num_error;
	data->num_error = 0;
	data->empty_str = NO; // flag to check if the string is empty (YES, NO)
	data->home_dir = getenv("HOME");
	// data->build_in = YES; // flag to check if it's a build in command (YES, NO)
	data->num_cmd = 0;
	// data->fd_pipe[0] = 0; something you need 
	// data->fd_pipe[1] = 0; same as above
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
		ts_parse(&data, line);
		// print_t_data(data);
		add_history(line);
		if (data.empty_str == NO)
		{
			ts_record_array(&data);
			//modify final cmd here
			for(int i = 0; i < data.num_cmd; i++)
				print_t_cmd(&data.cmd[i]);
			//execute here
			ts_free_all(&data, &line); // we will have to free the memory something like this 
		}
	}

}

void print_t_cmd(t_cmd *cmd)
{
    // printf("\033[1;35m t_cmd: \n\033[0m");
    // printf("  str: %s\n", cmd->str);
    // printf("  num_arg: %d\n", cmd->num_arg);
    // printf("  num_array_arg: %d\n", cmd->num_array_arg);
    // // printf("  way_cmd: %s\n", cmd->way_cmd);
    // printf("  count_redir: %d\n", cmd->count_redir);
    // // printf("  bad_file: %d\n", cmd->bad_file);
    // printf("  array_empty: %d\n", cmd->array_empty);
    printf("\033[1;35m everything inside the   cmd->arg.str :\n\033[0m");
    // for (int i = 0; i < cmd->num_arg; i++)
	// {
    //     // printf("    arg[%d]:\n", i);
    //     printf("      str: %s\n", cmd->arg[i].str);
    //     // printf("      q_m: %d\n", cmd->arg[i].q_m);
    //     printf("      space: %d\n", cmd->arg[i].space);
    //     printf("      redir: %d\n", cmd->arg[i].redir);
    //     printf("      empty_key: %d\n", cmd->arg[i].empty_key);
    // }

    printf(" \033[1;35m array_arg:\033[0m\n");
    for (int i = 0; cmd->array_arg[i] != NULL; i++)
	{
        printf("    array_arg[%d]: %s\n", i, cmd->array_arg[i]);
    }
	    
    printf("\033[1;35m  redir:\033[0m\n");
    for (int i = 0; i < cmd->count_redir; i++) 
	{
        printf("    redir[%d]: %d\n", i, cmd->redir[i]);
    }

    // printf("\033[1;35m  file:\033[0m\n");
    for (int i = 0; i < cmd->count_redir; i++) 
	{
        printf("    file[%d]: %s\n", i, cmd->file[i]);
    }
    // printf("\033[1;35m  fd: [%d, %d]\033[0m\n", cmd->fd[0], cmd->fd[1]);
    // printf("  redir_born: [%d, %d]\n", cmd->redir_born[0], cmd->redir_born[1]);
    // printf("  last_redir: %d\n", cmd->last_redir);
}
