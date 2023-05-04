#include "minishell.h"

void	print_tenv(t_env **list)
{
	if (!list)
	{
		printf("    (null)\n");
		return ;
	}
	if (!*list)
	{
		printf("    (list exists but is empty)\n");
		return ;
	}
	t_env *cur = *list;
	while (cur)
	{
		printf("KEY: %s; ", cur->key);
		printf("VAL: %s\n", cur->val);
		cur = cur->next;
	}
}

void	print_envlist_node(t_env *node)
{
	if (!node)
	{
		printf("(null)\n");
		return ;
	}
	if (node->key)
		printf("key: %s\n", node->key);
	if (node->val)
		printf("val: %s\n", node->val);
	printf("p: %i\n", node->p);
	printf("equal: %i\n", node->equal);
}

void	print_strlist(char **str)
{
	int i = 0;
	if (!str)
	{
		printf("    (null)\n");
		return ;
	}
	if (!*str)
	{
		printf("    the list exists but is empty\n");
		return ;
	}
	while (str[i])
	{
		printf("[%i]: %s\n", i, str[i]);
		if (str[i][0] == '\0')
			printf("malloc'd string that contains null in first index\n");
		i++;
	}
}

void	print_fd_array(int **fd, t_data *data)
{
	if (!fd)
	{
		printf("    (null)\n");
		return ;
	}
	if (!*fd)
	{
		printf("    the list exists but is empty\n");
		return ;
	}
	for (int i = 0; i < data->num_cmd - 1; i++)
	{
		for (int j = 0; j < 2; j++)
			printf("    fd[%i][%i] = %i ", i, j, data->fd[i][j]);
		printf("\n");
	}
}

void	print_pidlist(int *list, t_data *data)
{
	if (!list)
	{
		printf("    (null)\n");
		return ;
	}
	for (int i = 0; i < data->num_cmd; i++)
		printf("    pid[%i]: %i\n", i, data->pid[i]);
}

void	print_redirlist(int *list, t_cmd *cmd)
{
	if (!list)
	{
		printf("    (null)\n");
		return ;
	}
	for (int i = 0; i < cmd->count_redir; i++)
		printf("    redir[%i]: %i\n", i, cmd->redir[i]);
}

void	print_fd_cmd(int *list, t_cmd *cmd)
{
	if (!list)
	{
		printf("    (null)\n");
		return ;
	}
	for (int i = 0; i < cmd->count_redir; i++)
		printf("    fd[%i]: %i\n", i, cmd->fd_array[i]);
}

void	print_tcmd(t_cmd *cmd, int i)
{
	printf("*******CMD[%i]********\n", i);
	printf("\033[1;35m STR: \033[0m%s\n", cmd->str);
	// printf("\033[1;35m ARG->STR: \033[0m%s\n", cmd->arg->str);
	printf("\033[1;35m NUM_ARG: \033[0m%i\n", cmd->num_arg);
	printf("\033[1;35m COUNT_REDIR: \033[0m %i\n", cmd->count_redir);
	printf("\033[1;35m COUNT_HD: \033[0m%i\n", cmd->count_hd);
	printf("\033[1;35m RECORD_HD: \033[0m%i\n", cmd->record_hd);
	printf("\033[1;35m HEREDOC_STR: \033[0m%s\n", cmd->heredoc_str);
	printf("\033[1;35m LAST_INPUT: \033[0m%i\n", cmd->last_input);
	printf("\033[1;35m LAST_OUTPUT: \033[0m%i\n", cmd->last_output); 
	printf("\033[1;35m BUILTIN: \033[0m%i\n", cmd->builtin); 
	printf("\033[1;35m ARRAY_ARG: \033[0m\n");
	print_strlist(cmd->array_arg);
	printf("\033[1;35m PATH: \033[0m%s\n", cmd->path);
	printf("\033[1;35m REDIR: \033[0m\n");
	print_redirlist(cmd->redir, cmd);
	printf("\033[1;35m FILE: \033[0m\n");
	print_strlist(cmd->file);
	printf("\033[1;35m FD_ARRAY: \033[0m\n");
	print_fd_cmd(cmd->fd_array, cmd);
	printf("\033[1;35m HD_ARRAY: \033[0m\n");
	print_strlist(cmd->hd_array);
	// printf("\033[1;35m REDIR AND FILE: \033[0m\n");
	// for (int i = 0; i < cmd->count_redir; i++)
	// 	printf("    redir[%i]: %i ; file[%i]: %s\n", i, cmd->redir[i], i, cmd->file[i]);
	
}

void	print_tdata(t_data *data)
{	

	// printf("\033[1;35m NUM_ERROR: \033[0m%i\n", data->num_error);
	// printf("\033[1;35m NUM_PREV_ERROR: \033[0m%i\n", data->num_prev_error);	
	// printf("\033[1;35m NUM_ENV: \033[0m%i\n", data->num_env);
	// printf("\033[1;35m EMPTY_STR: \033[0m%i\n", data->empty_str);
	// printf("\033[1;35m NAME_FILE: \033[0m%i\n", data->name_file);
	// printf("\033[1;35m NUM_CMD: \033[0m%i\n", data->num_cmd);
	// printf("\033[1;35m OLD_DIR: \033[0m%s\n", data->old_dir);
	// printf("\033[1;35m CUR_DIR: \033[0m%s\n", data->cur_dir);
	printf("\033[1;35m ENV_LIST: \033[0m\n");
	print_tenv(data->env_list);
	// printf("\033[1;35m OUR_ENV: \033[0m\n");
	// print_strlist(data->our_env);
	// printf("\033[1;35m ENV_PATHS: \033[0m\n");
	// print_strlist(data->env_paths);
	// printf("\033[1;35m FD: \033[0m\n");
	// print_fd_array(data->fd, data);
	// printf("\033[1;35m PID: \033[0m\n");
	// print_pidlist(data->pid, data);
}

void	print_cmds(t_data *data)
{
	if (!data->cmd)
	{
		printf("    (null)\n");
		return ;
	}
	for (int i = 0; i < data->num_cmd; i++)
		print_tcmd(&data->cmd[i], i);
}