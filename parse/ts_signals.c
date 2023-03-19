/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 23:56:09 by jalwahei          #+#    #+#             */
/*   Updated: 2023/03/17 22:35:16 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sighandler(int signum)
{
	(void)signum;
	// rl_on_new_line();
	// rl_redisplay();
	write(1, "  \b\b\n", 5);
	rl_on_new_line();
	// rl_replace_line("", 1);
	rl_redisplay();
}

void	ts_signal_ctrl_d(t_data *data, char **line)
{
	if (*line == NULL)
	{
		printf("\033[1;35m\bTinyshell >\033[0A");
		printf("\033[1;0m exit\n\033[0m");
		// ts_free_all(data, line);
		exit(EXIT_SUCCESS);
	}
}


int	ts_get_signal(void)
{
	// signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
