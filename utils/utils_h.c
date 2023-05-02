#include "minishell.h"

 int ft_strcmp(const char *s1, const char *s2)
 {
	int	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	else if (!s1 && s2)
		return (0 - s2[i]);
	else if (s1 && !s2)
		return (s1[i] - 0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	return (s1[i] - s2[i]);
 }

 /* calloc with error management*/
void	*ts_calloc(size_t count, size_t size, t_data *data)
{
	void	*buf;

	buf = ft_calloc(count, size);
	if (!buf)
		error(ERR_MALLOC, data);
	return (buf);
}

/* dup2 with error management */
void	ts_dup2(int oldfd, int newfd, t_data *data)
{
	if (dup2(oldfd, newfd) == -1)
		error(ERR_DUP, data);
}

/* duplicate string up until but not including char c */
char	*ft_strdup_lim(const char *s1, char c, t_data *data)
{
	char	*ret_str;
	int		len;
	
	if (!s1)
		return (NULL);
	len = 0;
	while(s1[len] != c)
		len++;
	ret_str = (char *) ts_calloc(len + 1, sizeof(char), data);
	ft_strlcpy(ret_str, s1, len + 1);
	return (ret_str);
}

char	*ft_strjoin_char(char const *s1, char const *s2, char c, t_data *data)
{
	char	*buffer;
	int		len;
	int		i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)ft_calloc(len, sizeof(char));
	if (!buffer)
		error(ERR_MALLOC, data);
	i = ft_strlcpy(buffer, s1, len);
	buffer[i] = c;
	if (s2)
		ft_strlcat(buffer, s2, len);
	return (buffer);
}

char	**ts_split(char *str, char c, t_data *data)
{
	char	**buffer;

	buffer = ft_split(str, c);
	if (!buffer)
		error(ERR_MALLOC, data);
	return (buffer);
}

/* looks for char and returns index of that char if found. else it returns -1 */
int	detect_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

/* placeholder error function*/
void	error(int error, t_data *data)
{
	(void)data;
	if (error == ERR_MALLOC)
		write(2, "TinyShell: Allocation memory error\n", 35);
	else if (error == ERR_OPEN)
		write(2, "TinyShell: Open function error\n", 31);
	else if (error == ERR_PIPE)
		write(2, "TinyShell: Pipe function error\n", 31);
	else if (error == ERR_DUP)
		write(2, "TinyShell: Dup2 function error\n", 31);
	else if (error == ERR_FORK)
		write(2, "TinyShell: Fork function error\n", 31);
	else if (error == ERR_CWD)
		{
			write(2, "TinyShell: getcwd function error\n", 33);
		return ; // return 1 and update data to previous num error 
		}
	else if (error == ERR_EXEC)
		write(2, "TinyShell: exec function error\n", 31);
	else if (error == ERR_CLOSE)
		write(2, "TinyShell: close function error\n", 32);
	// close(data->defin);
	// close(data->defout);
	// free_data(data); // free if we exit 
	write(2, "Exiting TinyShell\n", 19); // for now
	exit(EXIT_FAILURE); // for now

}

/* prints strings and accounts for printf errors */
void	print_string(int num_str, ...)
{
	int		i;
	t_data	*data;
	va_list	str;
	va_start(str, num_str);
	i = 0;
	data = va_arg(str, t_data*);
	while (i < num_str)
	{
		if (printf("%s", va_arg(str, char *)) == -1)
			error(ERR_PRINT, data);
		i++;	
	}
	va_end(str);
}

int	putstr_fd(char *s, int fd)
{
	if (!s)
		return (0);
	while (*s)
		if (write(fd, s++, 1) == -1)
			return (-1);
	return(0);
}
/* prints strings and accounts for printf errors */
void	put_strs_fd(int num_str, ...)
{
	int		i;
	int		fd;
	t_data	*data;
	va_list	str;

	va_start(str, num_str);
	i = -1;
	data = va_arg(str, t_data*);
	fd = va_arg(str, int);
	while (++i < num_str)
	{
		if (putstr_fd(va_arg(str, char *), fd) == -1)
			error(ERR_PRINT, data);
	}
	va_end(str);
}

// if (fd == 1)
	// {
	// 	while (i < num_str)
	// 	{
	// 		if (printf("%s", va_arg(str, char *)) == -1)
	// 			error(ERR_PRINT, data);
	// 		i++;	
	// 	}
	// }