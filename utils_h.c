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
	char	*buf;

	buf = ft_calloc(count, size);
	if (!buf)
		error(data);
	return (buf);
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

	ret_split = (char **)ft_calloc_e(2, sizeof(char *), data);
	var_len = detect_char(str, '=');
	ret_split[0] = (char *)ft_calloc_e(var_len + 1, sizeof(char), data);
	val_len = ft_strlen(str) - (var_len + 1);
	ret_split[1] = (char *)ft_calloc_e(val_len + 1, sizeof(char), data);
	ft_strlcpy(ret_split[0], str, var_len + 1);
	str += (var_len + 1);
	ft_strlcpy(ret_split[1], str, val_len + 1);
	return (ret_split);
}