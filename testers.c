#include "minishell.h"

void	print_tcmd(t_cmd *cmd, int i)
{
	printf("*******CMD[%i]********\n", i);
	printf("\033[1;35m NUM_ARG: \033[0m%i\n", cmd->num_arg);
	printf("\033[1;35m COUNT_REDIR: \033[0m %i\n", cmd->count_redir);
	printf("\033[1;35m COUNT_HD: \033[0m%i\n", cmd->count_hd);
	printf("\033[1;35m RECORD_HD: \033[0m%i\n", cmd->record_hd);
	printf("\033[1;35m LAST_INPUT: \033[0m%i\n", cmd->last_input);
	printf("\033[1;35m LAST_OUTPUT: \033[0m%i\n", cmd->last_output); 
	printf("\033[1;35m ARRAY_ARG: \033[0m\n");
	for (int i = 0; i < cmd->num_arg; i++)
		printf("    array_arg[%i]: %s\n", i, cmd->array_arg[i]);
	printf("\033[1;35m PATH: \033[0m%s\n", cmd->path);
	if (cmd->count_redir > 0)
	{
		printf("\033[1;35m REDIR AND FILE: \033[0m\n");
		for (int i = 0; i < cmd->count_redir; i++)
			printf("    redir[%i]: %i ; file[%i]: %s\n", i, cmd->redir[i], i, cmd->file[i]);
		printf("\033[1;35m FD_ARRAY: \033[0m\n");
		for (int i = 0; i < cmd->count_redir; i++)
			printf("    %i ; ", cmd->fd_array[i]);
		printf("\n");
	}
	if (cmd->count_hd > 0)
	{
		printf("\033[1;35m HD_ARRAY: \033[0m\n");
		for (int i = 0; i < cmd->count_hd; i++)
			printf("    hd_array[%i]: %s\n", i, cmd->hd_array[i]);
	}
	printf("\n");
}

void	print_tenv(t_env **list)
{
	t_env *cur = *list;
	if (!list || !*list)
		printf("(null)\n");
	while (cur)
	{
		printf("    KEY: %s; ", cur->key);
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
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
}

void	print_tdata(t_data *data)
{
	printf("\033[1;35m NUM_CMD: \033[0m%i\n", data->num_cmd);
	printf("\033[1;35m CUR_DIR: \033[0m%s\n", data->cur_dir);
	printf("\033[1;35m NUM_ERROR: \033[0m%i\n", data->num_error);
	printf("\033[1;35m NUM_PREV_ERROR: \033[0m%i\n", data->num_prev_error);	
	printf("\033[1;35m NUM_ENV: \033[0m%i\n", data->num_env);
	printf("\033[1;35m EMPTY_STR: \033[0m%i\n", data->empty_str);
	printf("\033[1;35m NAME_FILE: \033[0m%i\n", data->name_file);
	printf("\033[1;35m ENV_LIST: \033[0m\n");
	print_tenv(data->env_list);
	printf("\033[1;35m OUR_ENV: \033[0m\n");
	for (int i = 0; data->our_env[i] != NULL; i++)
		printf("    %s\n", data->our_env[i]);
	printf("\033[1;35m ENV_PATHS: \033[0m\n");
	for (int i = 0; data->env_paths[i] != NULL; i++)
		printf("    %s\n", data->env_paths[i]);
	printf("\033[1;35m FD: \033[0m\n");
	for (int i = 0; i < data->num_cmd - 1; i++)
	{
		for (int j = 0; j < 2; j++)
			printf("    fd[%i][%i] = %i ", i, j, data->fd[i][j]);
		printf("\n");
	}
	printf("\033[1;35m PID: \033[0m\n");
	for (int i = 0; i < data->num_cmd; i++)
		printf("    pid[%i]: %i\n", i, data->pid[i]);
}