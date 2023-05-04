/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:01:44 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/03 22:02:41 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	detect_nflag(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	if (!str[i])
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	int		i;
	int		nflag;
	char	**arg;

	i = 1;
	nflag = 0;
	arg = cmd->array_arg;
	while (arg[i])
	{
		if (!detect_nflag(arg[i]))
			break ;
		nflag++;
		i++;
	}
	while (arg[i])
	{
		if (arg[i + 1] != NULL && i != 0)
			put_strs_fd(2, data, 1, arg[i], " ");
		else if (arg[i + 1] == NULL)
			put_strs_fd(1, data, 1, arg[i]);
		i++;
	}
	if (nflag == 0)
		put_strs_fd(1, data, 1, "\n");
	return (0);
}
