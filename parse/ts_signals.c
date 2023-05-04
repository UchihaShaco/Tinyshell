/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbui-vu <hbui-vu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 23:56:09 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 12:05:24 by hbui-vu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sighandler(int signum)
{
	(void)signum;
	rl_on_new_line();
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	ts_signal_ctrl_d(t_data *data, char **line)
{
	if (*line == NULL)
	{
		// printf("\033[1;35m\bTinyshell > \033[0A");
		// printf("\033[1;0mexit\n\033[0m");
		free_data(data, *line, YES);
		close(data->defin);
		close(data->defout);
		exit(data->num_prev_error);
	}
}

int	ts_get_signal(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

void	ts_free_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}

void	ts_init_emum_redir(t_cmd *cmd, int *i_orig, int num_redir)
{
	int	i;

	i = (*i_orig);
	if (cmd->str[i] == '>')
		cmd->redir[num_redir] = REDIR_W;
	else if (cmd->str[i] == '<')
		cmd->redir[num_redir] = REDIR_R;
	if (cmd->str[i] == '>' && cmd->str[i + 1] == '>')
	{
		cmd->redir[num_redir] = REDIR_W_ADD;
		i++;
	}
	else if (cmd->str[i] == '<' && cmd->str[i + 1] == '<')
	{
		cmd->redir[num_redir] = HEREDOC;
		i++;
	}
	else if (cmd->str[i] == '<' && cmd->str[i + 1] == '>')
	{
		cmd->redir[num_redir] = REDIR_RONG;
		i++;
	}
	i++;
	(*i_orig) = i;
}
