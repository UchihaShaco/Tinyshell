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

char	*init_str(char *input, t_data *data)
{
	char	*str;

	str = (char *)ft_calloc_e(ft_strlen(input) + 1, sizeof(char), data);
	ft_strlcpy(str, input, ft_strlen(input) + 1);
	free(input);
	return (str);
}

void	heredoc(char **delimiter_array, int record, t_data *data)
{
	char	*input;
	int		num_delimiters;
	int		i;
	char	*str;
	char	*new_str;

	str = NULL;
	num_delimiters = 0;
	while (delimiter_array[num_delimiters] != NULL)
		num_delimiters ++;
	i = 0;
	while (i < num_delimiters)
	{
		input = readline("> ");
		//while i = index of the delimiter, it has not hit the delimiter yet
		if (ft_strncmp(input, delimiter_array[i], ft_strlen(input)))
			i++;
		else if (i == num_delimiters - 1 && record == 1)
		{
			if (!str)
				str = init_str(input, data);
			else
			{
				new_str = ft_strjoin(str, input); //do we need to make a new function for this too
				free(str);
				free(input);
				str = new_str;
			}
		}
	}
	return (str);
}

int	heredoc_fd(char ** delimiter_array, int record, t_data *data)
{
	int		fd[2];
	char	*str;

	if (pipe(fd) == -1)
		error(data);
	str = here_doc(delimiter_array, record, data);
	write(fd[1], str, ft_strlen(str));
	free(str);
	close(fd[1]);
	return (fd[0]);
}