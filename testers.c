#include "minishell.h"

void	print_tcmd(t_cmd *cmd)
{
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