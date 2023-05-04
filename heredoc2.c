/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 07:51:22 by hbui-vu           #+#    #+#             */
/*   Updated: 2023/05/04 08:23:28 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_heredoc_fd(t_cmd *cmd, t_data *data)
{
	int		fd[2];
	char	*str;

	if (pipe(fd) == -1)
		error(ERR_PIPE, data);
	write(fd[1], cmd->heredoc_str, ft_strlen(cmd->heredoc_str));
	close(fd[1]);
	cmd->fd_array[cmd->last_input] = fd[0];
}
