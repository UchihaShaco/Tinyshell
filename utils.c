#include "minishell.h"

void	printarr(char **arr)
{
	while (*arr)
	{
		printf("%s\n", *arr);
		arr++;
	}
}

void	printlist(t_list *list)
{
	while (list)
	{
		printf("%s\n", (char *)list->content);
		list = list->next;
	}
}

t_list	*ft_arrtolst(char **arr)
{
	t_list	*list;

	list = NULL;
	while (*arr)
	{
		ft_lstadd_back(&list, ft_lstnew((void *)ft_strdup(*arr)));
		arr++;
	}
	return (list);
}

char	**ft_lsttoarr(t_list *list)
{
	char	**arr;
	int		i;

	arr = (char **)malloc(sizeof(char *) * (ft_lstsize(list) + 1));
	i = 0;
	while (list)
	{
		arr[i++] = ft_strdup((char *)list->content);
		list = list->next;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_copyarr(char **arr)
{
	char	**copy;
	int		len;

	len = ft_2dlen((void **)arr);
	copy = (char **)malloc(sizeof(char *) * (len + 1));
	copy[len] = NULL;
	while (len--)
		copy[len] = strdup(arr[len]);
	return (copy);
}

char	*listtostr(t_list *arr)
{
	char	*str;
	int		len;
	t_list	*start;

	len = 0;
	start = arr;
	while (arr)
	{
		len += ft_strlen((char *)arr->content);
		arr = arr->next;
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	len = 0;
	while (start)
	{
		ft_strcpy(&str[len], (char *)start->content);
		len += ft_strlen((char *)start->content);
		start = start->next;
	}
	return (str);
}

char	*arrtostr(char **arr)
{
	char	*str;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (arr[i])
	{
		len += ft_strlen(arr[i]);
		i++;
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	i = 0;
	len = 0;
	while (arr[i])
	{
		ft_strcpy(&str[len], arr[i]);
		len += ft_strlen(arr[i]);
		i++;
	}
	return (str);
}

int	ft_2dlen(void **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}
void	ms_malloc_cmd(t_cmd **cmd, int size)
{
	(*cmd) = malloc(sizeof(t_cmd) * (size + 1));
	if ((*cmd) == NULL)
	{
		write(2, "TinyShell: Allocation memory error\n", 35);
		exit(EXIT_FAILURE);
	}
}
int	find_last_redir(t_data *data, int i) // function to find the last redirection you might need it 
{
	int	j;

	j = 0;

	while (data->cmd[i].redir[j] != 0)
	{
		if (data->cmd[i].redir[j] == 3 || data->cmd[i].redir[j] == 4)
			data->cmd[i].last_redir = j;
		j++;
	}
	return (data->cmd[i].last_redir);
}
