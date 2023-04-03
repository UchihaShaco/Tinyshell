/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 19:09:51 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/01 04:41:31 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ts_free_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}

void	ts_free_arr(char ***arr)
{
	int	i;

	i = 0;
	if (**arr)
	{
		while ((*arr)[i] != NULL)
		{
			free((*arr)[i]);
			(*arr)[i] = NULL;
			i++;
		}
		if ((*arr)[i])
		{
			free((*arr)[i]);
			(*arr)[i] = NULL;
		}
		free(*arr);
		*arr = NULL;
	}
}

void	ts_free_int_arr(int **int_arr)
{
	int	i;

	i = 0;
	if (*int_arr)
	{
		free(*int_arr);
		*int_arr = NULL;
	}
}
