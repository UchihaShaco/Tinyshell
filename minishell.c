/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:56 by jalwahei          #+#    #+#             */
/*   Updated: 2023/03/10 16:33:33 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #define MAX_TOKENS 128

// typedef enum TokenType {
//     WORD,
//     PIPE,
//     REDIRECT_IN,
//     REDIRECT_OUT,
// };

// typedef struct Token {
//     enum TokenType	type;
//     char 			*value;
// } Token;

// void	parse_tokens(char *cmd, Token *tokens)
// {
// 	char	*p;
// 	int		i;

// 	p = cmd;
// 	i = 0;
// 	while (*p)
// 	{
//         	if (*p == ' ')
// 		{
// 			p++;
// 			continue ;
//         }
// 		if (*p == '|')
// 		{
// 			tokens[i].type = PIPE;
// 			tokens[i].value = p;
//             i++;
//             *p = 0;
//             p++;
//             continue;
//         }
//         if (*p == '<') {
//             tokens[i].type = REDIRECT_IN;
//             tokens[i].value = p;
//             i++;
//             *p = 0;
//             p++;
//             continue;
//         }
//         if (*p == '>') {
//             if (*(p + 1) == '>')
// 			{
//                 tokens[i].type = REDIRECT_OUT;
//                 tokens[i].value = p;
//                 i++;
//                 *p = 0;
//                 *(p + 1) = 0;
//                 p += 2;
//                 continue;
//             } else {
//                 tokens[i].type = REDIRECT_OUT;
//                 tokens[i].value = p;
//                 i++;
//                 *p = 0;
//                 p++;
//                 continue;
//             }
//         }
//         tokens[i].type = WORD;
//         tokens[i].value = p;
//         i++;
//         while (*p && *p != ' ' && *p != '|' && *p != '<' && *p != '>') {
//             p++;
//         }
//     }
//     tokens[i].value = NULL;
// }

// int		main(int argc, char *argv[])
// {
//     char cmd[1024];
//     Token tokens[MAX_TOKENS];

//     printf("Enter a command: ");
//     fgets(cmd, 1024, stdin);
//     cmd[strlen(cmd) - 1] = 0;

//     parse_tokens(cmd, tokens);

//     for (int i = 0; tokens[i].value; i++)
//     {
//         if (tokens[i].type == WORD)
//             printf("WORD: %s\n", tokens[i].value);
//         else if (tokens[i].type == PIPE)
//             printf("PIPE\n");
//         else if (tokens[i].type == REDIRECT_IN)
//             printf("REDIRECT_IN\n");
//         else if (tokens[i].type == REDIRECT_OUT)
//             printf("REDIRECT_OUT\n");
//     }

//     return (0);
// }
// 
// #include <string.h>

// static void	ts_argc_argv(int argc, char **argv, char **env)
// {
// 	if (argc != 1 || argv == NULL || env == NULL)
// 	{
// 		ft_putstr_fd("TinyShell: this programm complies without arguments\n", 2);
// 		exit(127);
// 	}
// }

// int	count_pipes(char *str, int max_pipes)
// {
// 	int	count;
// 	int	length;
// 	int	i;

// 	count = 0;
// 	i = 0;
// 	length = strlen(str);
// 	while (i < length)
// 	{
// 		if (str[i] == '|')
// 		{
// 			count++;
// 		}
// 		i++;
// 	}
// 	if (count > max_pipes)
// 	{
// 		return (0);
// 	}
// 	return (count);
// }

// int	main(int argc, char **argv, char **env)
// {
// 	char	*line;

// 	ts_argc_argv(argc, argv, env);
// 	while (1)
// 	{
// 		line = readline("\033[1;35mTinyShell > \033[0m");
// 		add_history(line);
// 	}
// }

// char	**parse_input(char *input)
// {
// 	char		**tokens;

// 	tokens = ft_split(input, ' ');
// 	return (tokens);
// }

// void	execute_command(char *cmd)
// {
// 	char		*args[100];
// 	char		**tokens;
// 	int			i;

// 	tokens = parse_input(cmd);
// 	i = 0;
// 	while (tokens[i] != NULL)
// 	{
// 		args[i] = tokens[i];
// 		i++;
// 	}
// 	args[i] = NULL;
// 	execv (args[0], args);
// }
// int	ms_parse(t_data *data, char *line)
// {
// 	int	i;

// 	i = 0;
// 	ms_count_and_record_cmd(data, line);
// 	if (data->num_error != 0 || data->empty_str == YES)
// 		return (-1);
// 	while (i < data->num_cmd)
// 	{
// 		if (data->num_error == 0)
// 			ms_found_redirect(&data->cmd[i], data);
// 		if (data
// 	i = 0;
// 	return (0);
// }
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
		// ts_init_env(data, env);
		data->name_file = NO;
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
int	ts_parse(t_data *data, char *line)
{
	int	i;

	i = 0;
	// int ik = 0;
	// int jk = 0;
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
		// ts_get_signal();b
		ts_init_data(&data, &env, NO);
		line = readline("\033[1;35mTinyShell > \033[0m");
		ts_signal_ctrl_d(&data, &line);
		ts_parse(&data, line);
		// print_t_data(data);
		add_history(line);
		// print_t_cmd(data.cmd);
		// 	if (data.empty_str == NO)
		// {
			ts_record_array(&data);
		for(int i = 0; i < data.num_cmd; i++)
		{
			print_t_cmd(&data.cmd[i]);
		}
		printf("num_cmd = %d\n", data.num_cmd);
		// 	ts_execution(&data, &line); // here what it should look like i think when executing :D
			// ts_free_cycle(&data, &line); // free the memory something like this 
		// }
	// 	if (strcmp(input, "exit") == 0) {
    //         break;
	// 	}
	// 	add_history(input);
	// 	strcpy(cmd, input);
	// 	pid = fork();
	// 	if (pid == 0)
	// 		execute_command(cmd);
	// 	else
	// 		wait(0);
	// 	free(input);
	// }
	// return (0);
}
}

void print_t_cmd(t_cmd *cmd)
{
    printf("\033[1;35m t_cmd: \n\033[0m");
    printf("  str: %s\n", cmd->str);
    printf("  num_arg: %d\n", cmd->num_arg);
    printf("  num_array_arg: %d\n", cmd->num_array_arg);
    // printf("  way_cmd: %s\n", cmd->way_cmd);
    printf("  count_redir: %d\n", cmd->count_redir);
    // printf("  bad_file: %d\n", cmd->bad_file);
    printf("  array_empty: %d\n", cmd->array_empty);

    printf("\033[1;35m everything inside the  arg:\n\033[0m");
    for (int i = 0; i < cmd->num_arg; i++) 
	{
        printf("    arg[%d]:\n", i);
        printf("      str: %s\n", cmd->arg[i].str);
        printf("      q_m: %d\n", cmd->arg[i].q_m);
        // printf("      space: %d\n", cmd->arg[i].space);
        // printf("      redir: %d\n", cmd->arg[i].redir);
        // printf("      empty_key: %d\n", cmd->arg[i].empty_key);
    }

    printf(" \033[1;35m array_arg:\033[0m\n");
    for (int i = 0; i < cmd->num_array_arg; i++) 
	{
        printf("    array_arg[%d]: %s\n", i, cmd->array_arg[i]);
    }

    printf("\033[1;35m  redir:\033[0m\n");
    for (int i = 0; i < cmd->count_redir; i++) 
	{
        printf("    redir[%d]: %d\n", i, cmd->redir[i]);
    }

    printf("\033[1;35m  file:\033[0m\n");
    for (int i = 0; i < cmd->count_redir; i++) 
	{
        printf("    file[%d]: %s\n", i, cmd->file[i]);
    }
    printf("\033[1;35m  fd: [%d, %d]\033[0m\n", cmd->fd[0], cmd->fd[1]);
    // printf("  redir_born: [%d, %d]\n", cmd->redir_born[0], cmd->redir_born[1]);
    printf("  last_redir: %d\n", cmd->last_redir);
}
