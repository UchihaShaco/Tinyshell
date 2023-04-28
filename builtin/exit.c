#include "../minishell.h"
/* NOTES
	exit 5a 5 bash: exit: abc: numeric argument required
	exit a bash: exit: abc: numeric argument required
	exit 5 5a bash: exit: too many arguments 255
	exit 50 (normal)exit
	exit 256 - 0
	bash: exit: 214748364887987987997: numeric argument required - takes maximum of long long
*/
//return last exit status
//exit out of shell
//if more than one argument, do not exit and send an error message
//or void????
//exit with multiple number arguments
//9223372036854775808

int	str_isalpha(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalpha(str[i]))
			return (1);
		i++;
	}
	return (0);
}

long long	check_exit_val(const char *str, t_data *data)
{
	unsigned long long	i;
	int					neg;

	i = 0;
	neg = 1;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			neg *= -1;
	while (ft_isdigit(*str))
	{
		if ((i > LLONG_MAX && neg == 1) || (i > LLMIN && neg == -1))
		{
			put_strs_fd(3, data, 2, "bash: exit: ", "str", ": numeric argument required\n");
			return (255);
		}
		i = (i * 10) + (*(str++) - 48);
	}
	put_strs_fd(1, data, 1, "exit\n");
	if (neg == -1)
		return (256-(i % 256));
	return (i % 256);
}

void	ft_exit(t_cmd *cmd, t_data *data)
{
	char	**arg;
	int		i;
	char	*line = malloc(5);

	arg = cmd->array_arg;
	if (!arg[1])
	{
		put_strs_fd(1, data, 1, "exit\n");
		i = 0;
	}
	else if (str_isalpha(arg[1]))
	{
		printf("str is alpha\n");
		put_strs_fd(3, data, 2, "bash: exit: ", arg[1], ": numeric argument required\n");
		i = 255;
	}
	else if (arg[2])
	{
		printf("there are multiple args\n");
		put_strs_fd(1, data, 2, "bash: exit: too many arguments\n");
		i = 255;
		return ;
	}
	else
		i = check_exit_val(arg[1], data);
		//alternatively, return the long long num and send it through exit.
	free_data(data, line, YES); // NEED TO EDIT
	exit(i);
}

