/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 04:29:03 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 04:29:03 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*init_str(char *input, t_data *data)
{
	char	*str;
	int		len;

	len = ft_strlen(input) + 2;
	str = (char *)ts_calloc(len, sizeof(char), data);
	ft_strlcpy(str, input, len);
	ft_strlcat(str, "\n", len);
	free(input);
	return (str);
}

/* strjoin specifically for heredocs */
char	*ft_strjoin_hd(char const *s1, char const *s2, t_data *data)
{
	char	*buffer;
	int		len;

	len = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)ts_calloc(len, sizeof(char), data);
	if (!s1)
		ft_strlcpy(buffer, s2, len);
	else
	{
		ft_strlcpy(buffer, s1, len);
		ft_strlcat(buffer, s2, len);
	}
	buffer[len - 2] = '\n';
	return (buffer);
}

void	herdoc_util(char **str, char *input, t_data *data, char **new_str)
{
	*new_str = ft_strjoin_hd(*str, input, data);
	if (*str)
		free(*str);
	*str = *new_str;
}

int	exit_signal(char *str)
{
	if (str)
		free(str);
	return (1);
}

void	get_heredoc_str(t_cmd *cmd, t_data *data, int i)
{
	char				*input;
	char				*str;
	char				*new_str;

	str = NULL;
	while (i < cmd->count_hd)
	{
		signal(SIGINT, hqhandle);
		input = readline("> ");
		if (g_hdsig == 42 && exit_signal(str) == 1)
			return ;
		if (!input && g_hdsig == 0)
			put_strs_fd(3, data, 1, "Wanted: '", cmd->hd_array[i++], "')\n");
		else if (input)
		{
			if (ft_strcmp(input, cmd->hd_array[i]) == 0)
				i++;
			else if (cmd->record_hd == 1 && i == cmd->count_hd - 1)
				herdoc_util(&str, input, data, &new_str);
		}
		if (input)
			free(input);
	}
	cmd->heredoc_str = str;
}
