#include "../minishell.h"
/* NOTES
	echo -nnnnn -na
	echo -nnnnna -n
	echo -nnnn -nn -n -na
	echo --nnnnnn -n -na
	echo -nnnnnnn -na -n
*/

/* returns 0 if not nflag, 1 if it is nflag */
int		detect_nflag(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	while (str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

int	ft_echo(t_cmd *cmd, t_data *data)
{
	int		i;
	int		nflag;
	char	**arg;

	i = 1;
	nflag = 0; 
	arg = cmd->array_arg;
	while (arg[i])
	{
		if (!detect_nflag(arg[i]))
			break;
		nflag++;
		i++;
	}
	while (arg[i])
	{
		if (arg[i + 1] != NULL)
			put_strs_fd(2, data, 1, arg[i], " ");
		else if (arg[i + 1] == NULL)
			put_strs_fd(1, data, 1, arg[i]);
		i++;
	}
	if (nflag == 0)
		put_strs_fd(1, data, 1, "\n");
	return (0);
}