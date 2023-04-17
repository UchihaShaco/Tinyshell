#include "minishell.h"

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

/* duplicate string up until a char c */
char	*ft_strdup_lim(const char *s1, char c, t_data *data)
{
	char	*ret_str;
	int		len;

	len = 0;
	while(s1[len] != c)
		len++;
	ret_str = (char *) ft_calloc(len + 1, sizeof(char));
	if (!ret_str)
		error(data);
	ft_strlcpy(ret_str, s1, len + 1);
	return (ret_str);
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
			error(data);
		i++;	
	}
	va_end(str);
}

/* calloc with error */
void	*ft_calloc_e(size_t count, size_t size, t_data *data)
{
	void	*buf;

	buf = ft_calloc(count, size);
	if (!buf)
		error(data);
	return (buf);
// 	void	*buffer;

// 	if (size != 0 && count > SIZE_MAX / size)
// 		error(data);
// 	buffer = malloc(count * size);
// 	if (!buffer)
// 	{
// 		printf("error\n");
// 		error(data);
// 	}
// 	ft_bzero(buffer, count * size);
// 	return (buffer);
}

/* placeholder error function */
void	error(t_data *data)
{
	(void)data;
	exit(1);
}

/* free an array of strings */
void	free_strlist(char **str)
{
	int	i;

	i = 0;
	if (str && *str)
	{
		while(str[i])
		{

			free(str[i]);
			i++;
		}
	}
	if (str)
		free(str);
}

/* split variable by only the first '=' and returns an array with key and value */
char	**split_env_var(char *str, t_data *data)
{
	int		var_len;
	int		val_len;
	char	**ret_split;
	ret_split = (char **)ft_calloc_e(3, sizeof(char *), data);
	var_len = detect_char(str, '=');
	ret_split[0] = (char *)ft_calloc_e(var_len + 1, sizeof(char), data);
	val_len = ft_strlen(str) - (var_len + 1);
	ret_split[1] = (char *)ft_calloc_e(val_len + 1, sizeof(char), data);
	ft_strlcpy(ret_split[0], str, var_len + 1);
	str += (var_len + 1);
	ft_strlcpy(ret_split[1], str, val_len + 1);
	ret_split[2] = NULL;
	return (ret_split);
}

/* get environmental paths */
void	get_env_paths(char **envp, t_data *data)
{
	int		i;
	// char	**env_paths;

	i = -1;
	while (envp[++i])
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
			break ;
	if (envp[i] == NULL)
		return ;
	while (*(envp[i]) != '/')
		envp[i]++;
	data->env_paths = ft_split(envp[i], ':');
	if (!data->env_paths)
		return ;
	// return (env_paths);
}

// void	get_env_paths(char **envp, t_data *data)
// {
// 	int		i;
// 	// char	**env_paths;

// 	i = -1;
// 	while (envp[++i])
// 		if (ft_strncmp("PATH=", envp[i], 5) == 0)
// 			break ;
// 	if (envp[i] == NULL)
// 		return ;
// 	while (*(envp[i]) != '/')
// 		envp[i]++;
// 	data->env_paths = ft_split(envp[i], ':');
// 	if (!data->env_paths)
// 		return ;
// 	// return (env_paths);
// }


/* get path */
void 	ts_add_cmd_path(char *arg, t_cmd *cmd,t_data *data)
{
	int		i;
	// char	*path;
	cmd->path = NULL;
	i = 0;
	// if (!arg || !detect_alnum(arg)) --> just detect one number or alphabet?????
	// 	return (NULL);
	if (access(arg, F_OK) == 0 && access(arg, X_OK) == 0)
	{
		cmd->path = ft_strdup_lim(arg, '\0', data);
		return ;
	}
	while (data->env_paths[i])
	{
		cmd->path = ft_strjoin_char(data->env_paths[i], arg, '/');
		if (access(cmd->path, F_OK) == 0 && access(cmd->path, X_OK) == 0)
			return ;
		else if (access(cmd->path, F_OK) == 0 && access(cmd->path, X_OK) < 0)
		{
			free(cmd->path);
			cmd->path = NULL;
			return ;
		}
		free(cmd->path);
		cmd->path = NULL;
		i++;
	}
}

/* join with a char */
char	*ft_strjoin_char(char const *s1, char const *s2, char c)
{
	char	*buffer;
	int		k;
	int		i;

	if (!s1)
		return (NULL);
	if (!s2)
		k = ft_strlen(s1) + 2;
	else
		k = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)ft_calloc(k, sizeof(char));
	if (!buffer)
		return (NULL);
	i = ft_strlcpy(buffer, s1, k);
	buffer[i] = c;
	if (s2)
		ft_strlcat(buffer, s2, k);
	return (buffer);
}

/* dup2 with error */
void	dup2_e(int oldfd, int newfd, t_data *data)
{
	if (dup2(oldfd, newfd) == -1)
		error(data);
}

/* print list of strings */
void	print_strlist(char **list)
{
	int	i;

	i = 0;
	if (!list)
		printf("(null)");
	while (list[i])
	{
		printf("%s\n", list[i]);
		i++;
	}
	// printf("\n");
}

/* ft_strjoin for heredoc with error management and addition of \n at the end */
char	*ft_strjoin_e(char const *s1, char const *s2, t_data *data)
{
	char	*buffer;
	int		k;

	if (!s1 || !s2)
		return (NULL);
	k = ft_strlen(s1) + ft_strlen(s2) + 2;
	buffer = (char *)malloc(sizeof(char) * k);
	if (!buffer)
		error(data);
	ft_strlcpy(buffer, s1, k);
	ft_strlcat(buffer, s2, k);
	ft_strlcat(buffer, "\n", k);
	return (buffer);
}
