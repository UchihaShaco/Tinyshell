/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 23:56:09 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/02 15:19:14 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sighandler(int signum)
{
	(void)signum;
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	ts_signal_ctrl_d(t_data *data, char **line)
{
	if (*line == NULL)
	{
		// printf("\033[1;35m\bTinyshell >\033[0A");
		// printf("\033[1;0mexit\n\033[0m");
		// ts_free_all(data, line);
		free_data(data, *line, YES);
		exit(EXIT_SUCCESS);
	}
}

void	ts_signal_ctrl_slash(t_data *data, char **line)
{
	if (*line)
	{
		printf("\033[1;35m\bTinyshell >\033[0A");
		printf("\033[1;0m exit\n\033[0m");
		// ts_free_all(data, line);
		exit(EXIT_SUCCESS);
	}
}


int	ts_get_signal(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
