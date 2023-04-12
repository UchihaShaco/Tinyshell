/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 10:52:58 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/11 15:51:08 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <stdarg.h>
# include <sys/wait.h>
# include "libft/libft.h"

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
	// ERR_Q_MARK, // unsused so far
	// ADD_TO_OLD, //might be helpful for exporting
	// ADD_NEW, // might be helpful for exporting
	DOUBLE_Q_MARK = 34,
	ONE_Q_MARK = 39,
	ERR_CMD = 127, // typically indicates that the specified command could not be found or executed by the shell.
	ERR_TOKEN = 258, // is not a standard exit code in bash, but some applications may use it to indicate a syntax error or unexpected token in the command line.
	ERR_NUM_ONE = 1, // can be used to indicate a generic error or failure.
	ERR_FILE_OR_DIR = 126, //126 typically indicates that the specified file or directory cannot be found or accessed by the shell.
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
	char	*path;
	int		*redir;
	char	**file;
	// int		fd[2]; // i think it will be used for piping
	// int		redir_born[2]; // i think it will be 
	int		last_redir; // last redir in the command
	int		num_arg;
	int		num_array_arg; // parsing part 
	int		count_redir; // c
	int		bad_file; // flag for error might need in excution (yes or no)
	int		array_empty;

}				t_cmd;

typedef struct	s_env
{
	char			*key;
	char			*val;
	int				p; //for printed
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_tmp
{
	int		size_str;
	int		size_cut;		
	int		count; 
}				t_tmp; 

typedef struct s_data
{
	t_cmd	*cmd;
	t_tmp	tmp;     // tmp struct to help with parsing and cutting file names
	t_env	**env_list;
	char	**env_paths;
	int		num_cmd; // number of commands
	int		num_error;  // error token ERR_TOKEN / DOUBLE_Q_MARK etc..
	int		num_prev_error; // to give exit value a number
	int		num_env;
	char	**our_env; 
	char	**tmp_var; // unsued empty tmp var
	int		num_tmp_var; // no need just an empty tmp var 
	char	*prev_dir; // previous directory
	char	*cur_dir; // current directory
	char	*home_dir; // home directory 
	int		flag_old; // something  could be used to keep track of prev direc
	int		empty_str; // flag for main function to know o execute or no
	int		**fd; // your pipes no idea how to use 😇 ;) --> we need to malloc this
	int		*pid;
	int		name_file; // not used so far
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
// void ts_quote_check(char *line);
/* *********************  Parse  ********************* */
int		ts_parse(t_data *data, char *line);
void	ts_check_empty_str(t_cmd *cmd);
/* ********************* Cmd Parse  ********************* */
int		ts_count_and_record_cmd(t_data *data, char *line);
/* *********************  parse redir  ********************* */
int		ts_error_parse_redir(t_data *data, char *s, int i);
int		ts_count_redirect(t_cmd *cmd, t_data *data, int qm_o, int qm_d);
int		ts_cycle_record_redir(t_cmd *cmd, t_data *data, int qm_o, int qm_d);
int		ts_found_redirect(t_cmd *cmd, t_data *data);
int		ts_record_redir_and_file(t_cmd *cmd, int i, int num_redir, t_data *d);
void	ts_init_emum_redir(t_cmd *cmd, int *i_orig, int num_redir);
/* *********************  parse Dollar  ********************* */
void	ts_found_dollar_in_name_file(t_data *data, char **file);
char	*ft_strdup_start(const char *s, int start);
void	ts_found_dollar(t_data *data, char **str, int q_m, int *i_orig);
int		ts_record_value(t_data *data, char **str, int i);
int		ts_search_var(t_data *data, char **value, char *key);
/* *********************  env  ********************* */
void	ts_init_env(t_data *data, char ***env);
int		ts_found_env_variable(t_data *data, t_cmd *cmd);
/* *********************  Signals  ********************* */
void	ts_signal_ctrl_d(t_data *data, char **line);
int		ts_get_signal(void);
void	ts_signal_ctrl_slash(t_data *data, char **line);
/* *********************  Malloc  ********************* */
void	ts_malloc_array(char ***array, int size);
void	ts_malloc_str(char **name, int size);
void	ts_malloc_arg(t_arg **arg, int size);
void	ts_malloc_cmd(t_cmd **cmd, int size);
void	ts_malloc_arr_int(int **arr_int, int size);
/* ********************* 	signals		  ********************* */
// apparently i need to include it in the header file to use it otherwise it wont compile  even tho i have its library included
void	rl_replace_line(const char *text, int clear_undo); 
/* ********************* 	error msgs  ********************* */
int		ts_error(int error, char *str);
int		ts_err_token(t_data *data, int pipe);
int		ts_error_2(int error, int qm);
int		print_error_pipe(int error, char *str);
/* ********************* 	Utils for parse  ********************* */
void	ts_free_str(char **str);
int		ts_record_key(char *s, int i, char **key, int *digit_key);
void	ts_record_char(char **result, char *str, int *r, int *s);
void	ts_record_tail(char **tmp, char **str, int t, int start_tail);
void	ts_record_str(char **file, char *str, int start, int size_str);
void	ts_replace_key_to_value(char **str, int key, char *value, int start);
/* ********************* 	file name ********************* */
int		ts_measure_size_file_name(t_data *d, char *str, int *i);
/* ********************* 	record arr  ********************* */
void	ts_record_array(t_data *data);
/* ********************* 	printer of the data to test  ********************* */
void	print_t_data(struct s_data data);
// void	print_arg(t_arg *arg);
void	print_t_cmd(t_cmd *cmd);

/* BUILTINS */
void	ft_cd(char **arg, t_data *data);
void	ft_echo(char **str, t_data *data);
void	ft_env(t_data *data);
void	ft_export(char **arg, t_data *data);
void	ft_pwd(t_data *data);
void	ft_unset(char **arg, t_data *data);
void	execute_builtin(char **arg, int	i, t_data *data);
int		check_builtin(char *str, t_data *data);

/* ENV VAR */
t_env	*find_var_envlist(char *key, t_data *data);
void	modify_our_env(t_env *env_var, t_data *data);
void	add_env_var(char *key, char *val, t_data *data);

/* UTILS */
int		detect_char(char *str, char c);
void	print_string(int num_str, ...);
void	error(t_data *data);
void	print_string(int num_str, ...);
void	*ft_calloc_e(size_t count, size_t size, t_data *data);
char	*ft_strdup_lim(const char *s1, char c, t_data *data);
void	free_strlist(char **str);
char	**split_env_var(char *str, t_data *data);
void	get_env_paths(char **envp, t_data *data);
void	ts_add_cmd_path(char *arg, t_cmd *cmd,t_data *data);
char	*ft_strjoin_char(char const *s1, char const *s2, char c);

/* EXECUTION */
int	pipex(t_data *data);

// /* TESTERS */
// void	print_strlist(char **list);
// void	print_data(t_data *data);

#endif