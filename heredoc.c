//For heredocs
//keep track of delimiters 
//all delimiters must be typed out before exiting
//last delimiter must be at the end
//delimiters must be written in order
//only text written after the last delimiter is considered to be input



//keep track of heredoc delimiter in an array
//if there a heredoc redirector, will have to open up terminal for writing
//use readline to print > everytime something is written
//heredoc delimiters must be written IN ORDER for it to exit out
//if more than one heredoc delimiter
//disregard everything written until second to the last delimiter passes
//start writing to temporary file everything after that 2nd to last delimiter
//unlink temporary file -> in close function??? does fd_array have to be a struct???????
//other option is to create a temporary pipe - write to pipe and have command read from read end 

#include "minishell.h"

int	count_delimiters(t_cmd *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < cmd->count_redir)
	{
		if (cmd->redir[i] == 5)
			count++;
		i++;
	}
	return (count);
}

char	*init_str(char *input, t_data *data)
{
	char	*str;
	int		len;

	len = ft_strlen(input) + 2;

	str = (char *)ft_calloc_e(len, sizeof(char), data);
	ft_strlcpy(str, input, len);
	ft_strlcat(str, "\n", len);
	free(input);
	return (str);
}

char	*generate_heredoc(t_cmd *cmd, int record_hd, t_data *data)
{
	char	*input;
	char	*str;
	char	*new_str;
	int		i;

	str = NULL;
	while (i < cmd->count_hd)
	{
		input = readline("> ");
		//while i = index of the delimiter, it has not hit the delimiter yet
		
		if (ft_strncmp(input, cmd->hd_array[i], ft_strlen(input)) == 0)
			i++;
		else if (i == cmd->count_hd - 1 && record_hd == 1)
		{
			if (!str)
				str = init_str(input, data);
			else
			{
				new_str = ft_strjoin_e(str, input, data); //do we need to make a new function for this too for malloc error
				free(str);
				free(input);
				str = new_str;
			}
		}
	}
	printf("the final string is: %s\n", str);
	return (str);
}

void	get_heredoc_fd(t_cmd *cmd, int record_hd, t_data *data)
{
	int		fd[2];
	char	*str;

	str = NULL;
	//only create pipe if we have to input a heredoc
	if (record_hd == 1)
		if (pipe(fd) == -1)
			error(data);
	str = generate_heredoc(cmd, record_hd, data);
	if (record_hd == 1)
	{
		write(fd[1], str, ft_strlen(str));
		free(str);
		close(fd[1]);
		cmd->fd_array[cmd->last_input] = fd[0];
	}
}