/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 04:48:38 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 07:31:41 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ts_malloc_array(char ***array, int size)
{
	(*array) = (char **)ft_calloc(sizeof(char *), (size + 1));
	if ((*array) == NULL)
	{
		write(2, "Tinyshell: Allocation memory error\n", 35);
		exit(EXIT_FAILURE);
	}
}

void	ts_malloc_str(char **name, int size)
{
	(*name) = ft_calloc(sizeof(char), (size + 1));
	if ((*name) == NULL)
	{
		write(2, "Tinyshell: Allocation memory error\n", 35);
		exit(EXIT_FAILURE);
	}
}
/*stupid conditional jump had to be fixed here intitalizing space 😒😒😒 */

void	ts_malloc_arg(t_arg **arg, int size)
{
	(*arg) = ft_calloc(sizeof(t_arg), (size + 1));
	while (size >= 0)
	{
		(*arg)[size].space = NO;
		size--;
	}
	if ((*arg) == NULL)
	{
		write(2, "Tinyshell: Allocation memory error\n", 35);
		exit(EXIT_FAILURE);
	}
}
//DO WE NEED SIZE + 1

void	ts_malloc_cmd(t_cmd **cmd, int size)
{
	(*cmd) = ft_calloc(sizeof(t_cmd), (size));
	if ((*cmd) == NULL)
	{
		write(2, "Tinyshell: Allocation memory error\n", 35);
		exit(EXIT_FAILURE);
	}
}

void	ts_malloc_arr_int(int **arr_int, int size)
{
	(*arr_int) = (int *)ft_calloc(sizeof(int), (size));
	if ((*arr_int) == NULL)
	{
		write(2, "Tinyshell: Allocation memory error\n", 35);
		exit(EXIT_FAILURE);
	}
}
//DO WE NEED SIZE + 1?
