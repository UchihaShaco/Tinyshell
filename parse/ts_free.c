/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 19:09:51 by jalwahei          #+#    #+#             */
/*   Updated: 2023/04/28 22:21:39 by hbui-vu          ###   ########.fr       */
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
	if (*int_arr)
	{
		free(*int_arr);
		*int_arr = NULL;
	}
}

void	ts_free_all(t_data *data, char **line)
{
	if (data->our_env != NULL)
		ts_free_arr(&data->our_env);
	ts_free_cycle(data, line);
	ts_free_str(&data->old_dir);
	ts_free_str(&data->cur_dir);
		// if (data->tmp_var != NULL)
	// 	ts_free_arr(&data->tmp_var);
}

void	ts_free_arg(t_data *data, int y)
{
	int	x;

	x = 0;
	if (data->cmd[y].num_arg > 0)
	{
		while (x < data->cmd[y].num_arg)
		{
			ts_free_str(&data->cmd[y].arg[x].str);
			x++;
		}
		free(data->cmd[y].arg);
		data->cmd[y].arg = NULL;
	}
}

void	ts_free_cycle(t_data *data, char **line)
{
	int	y;

	y = 0;
	ts_free_str(line);
	if (data->num_cmd > 0)
	{
		while (y < data->num_cmd)
		{
			ts_free_arg(data, y);
			if (data->cmd[y].str != NULL)
				ts_free_str(&data->cmd[y].str);
			if (data->cmd[y].array_empty == NO)
				ts_free_arr(&data->cmd[y].array_arg);
			if (data->cmd[y].count_redir > 0)
			{
				ts_free_int_arr(&data->cmd[y].redir);
				ts_free_arr(&data->cmd[y].file);
			}
			// if (data->cmd[y].way_cmd != NULL) // what ever name it is 
			// 	ts_free_str(&data->cmd[y].way_cmd); // what ever name it is
			y++;
		}
		free(data->cmd);
		data->cmd = NULL;
	}
}
