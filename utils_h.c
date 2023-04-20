#include "minishell.h"

 int ft_strcmp(const char *s1, const char *s2)
 {
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break;
		i++;
	}
	return (s1[i] - s2[i]);
 }

 /* calloc with error */
void	*ts_calloc(size_t count, size_t size, t_data *data)
{
	void	*buf;

	buf = ft_calloc(count, size);
	if (!buf)
		error(MALLOC_ERR, data);
	return (buf);
}

/* duplicate string up until a char c */
char	*ft_strdup_lim(const char *s1, char c, t_data *data)
{
	char	*ret_str;
	int		len;

	len = 0;
	while(s1[len] != c)
		len++;
	ret_str = (char *) ts_calloc(len + 1, sizeof(char), data);
	ft_strlcpy(ret_str, s1, len + 1);
	return (ret_str);
}

/* placeholder error function*/
void	error(int error, t_data *data)
{
	(void)data;
	if (error = ERR_MALLOC)
		write(2, "Tinyshell: Allocation memory error\n", 35);
	//free(data);
	exit(EXIT_FAILURE);
}