/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 10:52:58 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 12:03:13 by hbui-vu          ###   ########.fr       */
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
# include <stdint.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdarg.h>
# include <limits.h>
# include <sys/stat.h>
# include <errno.h>

int	g_hdsig;

typedef enum e_value
{
	NO,
	YES,
	REDIR_R,
	REDIR_W,
	REDIR_W_ADD,
	HEREDOC,
	REDIR_RONG,
	ERR_PIPE,
	ERR_MALLOC,
	ERR_OPEN,
	ERR_DUP,
	ERR_EXEC,
	ERR_FORK,
	ERR_PRINT,
	ERR_CWD,
	ERR_CLOSE,
	PARENT,
	CHILD,
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
	int		q_m;
	int		space;
	int		redir;
	int		empty_key;

}				t_arg;

typedef struct s_cmd
{
	char	*str;
	t_arg	*arg;
	int		num_array_arg;
	int		array_empty;
	char	**array_arg;
	char	**hd_array;
	char	**file;
	char	*heredoc_str;
	char	*path;
	int		*redir;
	int		*fd_array;
	int		num_arg;
	int		old_num_arg;
	int		count_redir;
	int		count_hd;
	int		record_hd;
	int		last_output;
	int		last_input;
	int		builtin;
}				t_cmd;

typedef struct s_tmp
{
	int		size_str;
	int		size_cut;		
	int		count;
}				t_tmp;

typedef struct s_env
{
	char			*key;
	char			*val;
	int				p;
	int				equal;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_data
{
	t_tmp	tmp;
	int		num_error;
	int		num_prev_error;
	int		num_env;
	int		empty_str;
	int		name_file;
	t_cmd	*cmd;
	t_env	**env_list;
	char	**our_env;
	char	**env_paths;
	char	*old_dir;
	char	*cur_dir;
	char	*line;
	int		**fd;
	int		*pid;
	int		num_cmd;
	int		defin;
	int		defout;
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
int		ts_check_quotation_marks(t_cmd *cmd, int i, t_data *data);
// void ts_quote_check(char *line);
/* *********************  Parse  ********************* */
void	ts_parse(t_data *data, char *line);
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
/* *********************  Malloc  ********************* */
void	ts_malloc_array(char ***array, int size);
void	ts_malloc_str(char **name, int size);
void	ts_malloc_arg(t_arg **arg, int size);
void	ts_malloc_cmd(t_cmd **cmd, int size);
void	ts_malloc_arr_int(int **arr_int, int size);

/* ********************* 	signals		  ********************* */
void	hqhandle(int sig);
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
void	ts_replace_key_to_value(char **str, int key, \
char *value, int start);
/* ********************* 	file name ********************* */
int		ts_measure_size_file_name(t_data *d, char *str, int *i);
/* ********************* 	record arr  ********************* */
void	ts_record_array(t_data *data);

/* UTILS */
int		ft_strcmp(const char *s1, const char *s2);
void	*ts_calloc(size_t count, size_t size, t_data *data);
char	**ts_split(char *str, char c, t_data *data);
void	ts_dup2(int oldfd, int newfd, t_data *data);
char	*ft_strdup_lim(const char *s1, char c, t_data *data);
char	*ft_strjoin_char(char const *s1, char const *s2, char c, t_data *data);
int		detect_char(char *str, char c);
void	print_string(int num_str, ...);
int		putstr_fd(char *s, int fd);
void	put_strs_fd(int num_str, ...);
void	error(int error, t_data *data);
void	ts_err_argc_argv(int argc, char **argv, char **env);
void	close_fd_array(t_cmd *cmd, t_data *data);
void	close_pipes(t_data *data);
int		err(char *str, int errno, int proc, t_data *data);
int		err_open(t_cmd *cmd, int proc, t_data *data);
void	redir5(t_cmd *cmd, t_data *data, int i);
void	mod_redir_doubles(t_cmd *cmd);
void	redir_utils(char ***new_file, int *new_redir, t_cmd *cmd, \
	int new_count_redir);
int		find_path_separator(t_cmd *cmd);
void	check_redir_doubles(t_cmd *cmd, t_data *data);
void	init_heredoc(t_cmd *cmd, t_data *data);
void	as_dir_utils(t_cmd *cmd, t_data *data, struct stat file_stat);
void	check_as_dir(t_cmd *cmd, t_data *data);
int		check_permissions_executable(t_cmd *cmd, t_data *data);
void	chk_cmd_utils(t_cmd *cmd, t_data *data, struct stat file_stat);
void	check_as_command(t_cmd *cmd, t_data *data);
/* FREE */
void	free_strlist(char **str);
void	free_data(t_data *data, char *line, int last);
void	free_util(t_data *data, char *line);
void	free_fdlist(t_data *data);
void	free_cmd(t_data *data);

/* ENV */
t_env	*find_var_envlist(char *key, t_data *data);
void	add_var_envlist(t_env *node, t_data *data);
char	**split_var_envlist(char *str, t_data *data, int plus);
void	init_envlist(t_data *data, char **envp);
void	rewrite_ourenv(t_data *data);
void	clear_envlist(t_env **env_list);
void	delete_var_envlist(char *key, t_data *data);
t_env	*create_var_envlist(char **var, t_data *data);

/* BUILTIN */
int		ft_echo(t_cmd *cmd, t_data *data);
int		ft_export(t_cmd *cmd, t_data *data);
int		ft_env(t_data *data);
int		ft_pwd(t_data *data);
int		ft_unset(t_cmd *cmd, t_data *data, int i);
int		ft_cd(t_cmd *cmd, t_data *data);
int		ft_exit(t_cmd *cmd, char *line, t_data *data);
void	print_export(t_data *data);
int		check_exp_entry(char *str);
void	exp_concat(char *str, int *rewr, t_data *data);
t_env	*find_next_print(t_data *data);
t_env	*create_pwd_envlist(char **var, t_data *data);
t_env	*find_next_print(t_data *data);
/* HEREDOC */
void	get_heredoc_fd(t_cmd *cmd, t_data *data);
void	get_heredoc_str(t_cmd *cmd, t_data *data, int i);

/* DATA & CMD*/
void	finalize_cmd(t_data *data);
void	init_env_list(t_data *data, char **envp);
void	get_env_paths(t_data *data);
char	*find_home_dir(t_data *data);
void	create_fd_pid_array(t_data *data);

/* EXECUTION */
void	execute(char *line, t_data *data);
int		check_builtin(t_cmd *cmd, t_data *data);
int		execute_builtin(t_cmd *cmd, int proc, char *line, t_data *data);
void	get_cmd_path(t_cmd *cmd, t_data *data);
int		open_files(t_cmd *cmd, int proc, t_data *data);
void	pipe_cmd(int index, t_cmd *cmd, t_data *data);
int		find_path_separator(t_cmd *cmd);
void	qhandler(int sig);

/* PARSING */
void	mod_redir_doubles(t_cmd *cmd);
void	redir_utils(char ***new_f, int *new_r, t_cmd *cmd, int new_ct_redir);
void	make_new_arrays(t_cmd *cmd, char ***new_f, int **new_rdir, \
	t_data *data);
void	check_redir_doubles(t_cmd *cmd, t_data *data);
void	check_hd_last_redir(t_cmd *cmd);

#endif