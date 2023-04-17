#include "minishell.h"

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
	fflush(stdout);
    for (int i = 0; cmd->array_arg[i] != NULL; i++)
	{
        printf("    array_arg[%d]: %s\n", i, cmd->array_arg[i]);
        printf("    array_path: %s\n", cmd->path);
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

}