#include "minishell.h"

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
	str = NULL;
}