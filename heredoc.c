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

void	get_heredoc_str(t_cmd *cmd, t_data *data)
{
	char	*input;
	char	*str;
	char	*new_str;
	int		i;

	str = NULL;
	i = 0;
	//while i = index of the delimiter, it has not hit the delimiter yet
	while (i < cmd->count_hd)
	{
		// signal(SIGINT, SIG_IGN);
		input = readline("> ");
		if(!input) //instead jump to the next delimiter
		{
			put_strs_fd(3, data, 1, "bash: warning: here-document delimited by end-of-file (wanted '", cmd->hd_array[i], "')\n");
			i++;
		}
		else
		{
			if (ft_strcmp(input, cmd->hd_array[i]) == 0)
				i++;
			else if (cmd->record_hd == 1 && i == cmd->count_hd - 1)
			{
				new_str = ft_strjoin_hd(str, input, data);
				if (str)
					free(str);
				str = new_str;
			}
		}
		free(input);
	}
	cmd->heredoc_str = str;
}

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