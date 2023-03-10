/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 10:52:58 by jalwahei          #+#    #+#             */
/*   Updated: 2023/03/10 16:37:11 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>

typedef enum e_value
{
	NO,
	YES,
	REDIR_R, // redirect read '<'
	REDIR_W, // redirect write '>'
	REDIR_W_ADD, // redirect add (append) '>>'
	HEREDOC, // heredoc '<<'
	REDIR_RONG,
	// YES_AFTER_PARSER, // unsused so far
	ERR_PIPE, 
	// ERR_MEMORY, // unsused so far 
	ERR_Q_MARK,
	// ADD_TO_OLD, //might be helpful for exporting
	// ADD_NEW, // might be helpful for exporting
	DOUBLE_Q_MARK = 34,
	ONE_Q_MARK = 39,
	ERR_CMD = 127,
	ERR_TOKEN = 258,
	ERR_NUM_ONE = 1,
	ERR_FILE_OR_DIR = 126,
}				t_value;

typedef struct s_arg
{
	char	*str;
	int		q_m; // flag to know if theres qm if its a NO it means it cleared 
	int		space; // flag to know if there was a space after the arg but i removed them all  so it might be useless i didnt need it  but thought might be helpful  dont judge me :( 
	int		redir;  // idk might be usefl later for parsing
	int		empty_key; // idk why i did this but i think it was for the envp and $ sign maybe

}				t_arg;

typedef struct s_cmd
{
	char	*str; // saving command as a string
	t_arg	*arg;
	char	**array_arg;
	int		*redir;
	char	**file;
	int		fd[2]; // with redir_born[2] i think it will be used for piping
	int		redir_born[2]; // i think it will be used along with fd[2] 
	int		last_redir; // last redir in the command
	int		num_arg;
	int		num_array_arg; // 
	char	*way_cmd; // path to cmd
	int		count_redir; // c
	int		bad_file; // flag for error might need in excution (yes or no)
	int		array_empty;

}				t_cmd;

typedef struct s_tmp
{
	int		size_str;
	int		size_cut;		
	int		count; 
}				t_tmp; 

typedef struct s_data
{
	t_cmd	*cmd;
	t_tmp	tmp; 
	int		num_cmd; // number of commands
	int		num_error;  // error token ERR_TOKEN / DOUBLE_Q_MARK etc..
	int		num_prev_error; // to give exit value a number
	int		num_env;
	char	**our_env;
	char	**tmp_var;
	int		num_tmp_var; // no need
	char	*prev_dir; // previous directory
	char	*cur_dir; // current directory
	char	*home_dir; // home directory 
	int		flag_old; // something  could be used to keep track of prev direc
	int		empty_str; // flag for parsing to know if string is empty or not (YES,NO)
	int		fd_pipe[2]; // your pipes no idea how to use ???? ;)
	int		name_file;
	int		build_in; // flag to know if its a build in cmd (YES,NO)
	// int		*pid;
	int		n_end;
}				t_data;
/* *********************  Quotation parse  ********************* */
void	ts_create_struct_without_qm(t_cmd *cmd);
int		ts_count_arg_divided_qm(t_cmd *cmd, t_data *data);
int		ts_record_args_without_qm(char *str, t_arg *arg, int i, int *num_arg);
int		ts_cut_quotation_marks(char *str, t_arg *arg, int i);
void	ts_search_space_after_arg(char *str, t_arg *arg, int i);
void	ts_switch_qm(char c, int *qm_o, int *qm_d);
int		ts_cut_qm_in_name_file(char **file);
int		ts_count_args_without_qm(t_cmd *cmd, int i);
int		ts_count_arg_divided_qm(t_cmd *cmd, t_data *data);
int		ts_check_quotation_marks(t_cmd *cmd, int i, t_data *data);

/* *********************  Parse  ********************* */
int		ts_parse(t_data *data, char *line);
void	ts_check_empty_str(t_cmd *cmd);
/* ********************* Cmd Parse  ********************* */
int		ts_get_size_one_cmd_str(char *line, int *start, int size);
int		ts_record_one_str(char **str, char *line, int *start, int *num);
int		ts_count_and_record_cmd(t_data *data, char *line);
int		ts_check_empty_and_err_token_pipe(t_data *data, char *line);
/* *********************  parse redir  ********************* */
int		ts_error_parse_redir(t_data *data, char *s, int i);
int		ts_count_redirect(t_cmd *cmd, t_data *data, int qm_o, int qm_d);
int		ts_cycle_of_record_redir(t_cmd *cmd, t_data *data, int qm_o, int qm_d);
int		ts_found_redirect(t_cmd *cmd, t_data *data);
int		ts_record_redir_and_file(t_cmd *cmd, int i, int num_redir, t_data *d);
void	ts_init_emum_redir(t_cmd *cmd, int *i_orig, int num_redir);
/* *********************  env  ********************* */
void	ts_init_env(t_data *data, char ***env);
/* *********************  Signals  ********************* */
void	ts_signal_ctrl_d(t_data *data, char **line);
int		ts_get_signal(void);

/* *********************  Malloc  ********************* */
void	ts_malloc_array(char ***array, int size);
void	ts_malloc_str(char **name, int size);
void	ts_malloc_arg(t_arg **arg, int size);
void	ts_malloc_cmd(t_cmd **cmd, int size);
void	ts_malloc_arr_int(int **arr_int, int size);
/* ********************* 	error msgs  ********************* */
int		ts_error(int error, char *str);
int		ts_err_token(t_data *data, int pipe);
int		ts_error_2(int error, int qm);
int		print_error_pipe(int error, char *str);
/* ********************* 	Utils for parse  ********************* */
void	ts_free_str(char **str);
void	ts_replace_key_to_value(char **str, int key, char *value, int start);
int		ts_record_key(char *s, int i, char **key, int *digit_key);
void	ts_record_char(char **result, char *str, int *r, int *s);
void	ts_record_tail(char **tmp, char **str, int t, int start_tail);
void	ts_record_str(char **file, char *str, int start, int size_str);
/* ********************* 	file name ********************* */
int	ts_measure_size_file_name(t_data *d, char *str, int *i);
/* ********************* 	record arr  ********************* */
void	ts_record_array(t_data *data);
/* ********************* 	printer of the data to test  ********************* */
void	print_t_data(struct s_data data);
// void	print_arg(t_arg *arg);
void	print_t_cmd(t_cmd *cmd);
#endif