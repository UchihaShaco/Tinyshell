/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 04:48:38 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/29 16:23:56 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ts_malloc_array(char ***array, int size, t_data *data)
{
	// (*array) = (char **)malloc(sizeof(char *) * (size + 1));
	*array = (char **)ts_calloc(size + 1, sizeof(char *), data);
	// if ((*array) == NULL)
	// {
	// 	write(2, "Tinyshell: Allocation memory error\n", 35);
	// 	exit(EXIT_FAILURE);
	// }
}

void	ts_malloc_str(char **name, int size, t_data *data)
{
	// (*name) = malloc(sizeof(char) * (size + 1));
	// if ((*name) == NULL)
	// {
	// 	write(2, "Tinyshell: Allocation memory error\n", 35);
	// 	exit(EXIT_FAILURE);
	// }
	*name = (char *)ts_calloc(size + 1, sizeof(char), data);
}
/*stupid conditional jump had to be fixed here intitalizing space 😒😒😒 */

void	ts_malloc_arg(t_arg **arg, int size, t_data *data)
{
	// (*arg) = malloc(sizeof(t_arg) * (size + 1));
	// while (size >= 0)
	// {
	// 	(*arg)[size].space = NO;
	// 	size--;
	// }
	// if ((*arg) == NULL)
	// {
	// 	write(2, "Tinyshell: Allocation memory error\n", 35);
	// 	exit(EXIT_FAILURE);
	// }
	*arg = (t_arg *)ts_calloc(size + 1, sizeof(t_arg), data);
}
//DO WE NEED SIZE + 1

void	ts_malloc_cmd(t_cmd **cmd, int size, t_data *data)
{
	// (*cmd) = malloc(sizeof(t_cmd) * (size + 1));
	(*cmd) = ts_calloc(size, sizeof(t_cmd), data);
	// if ((*cmd) == NULL)
	// {
	// 	write(2, "Tinyshell: Allocation memory error\n", 35);
	// 	exit(EXIT_FAILURE);
	// }
}

void	ts_malloc_arr_int(int **arr_int, int size, t_data *data)
{
	// (*arr_int) = (int *)malloc(sizeof(int) * (size + 1));
	// if ((*arr_int) == NULL)
	// {
	// 	write(2, "Tinyshell: Allocation memory error\n", 35);
	// 	exit(EXIT_FAILURE);
	// }
	*arr_int = (int *)ts_calloc(size, sizeof(int), data);
}
//DO WE NEED SIZE + 1?
