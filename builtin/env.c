/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 22:03:10 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/03 22:03:10 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_data *data)
{
	char	**our_env;
	int		i;

	our_env = data->our_env;
	if (!our_env)
		return (0);
	i = 0;
	while (our_env[i])
	{
		put_strs_fd(2, data, 1, our_env[i], "\n");
		i++;
	}
	return (0);
}
