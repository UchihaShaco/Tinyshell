#include "../minishell.h"
/* TESTS:
	exit 5a 5 -> bash: exit: abc: numeric argument required
	exit a -> bash: exit: abc: numeric argument required
	exit 5 5a -> bash: exit: too many arguments 255
	exit 50 -> exit
	exit 256 -> exit; echo $? = 0
	bash: exit: 214748364887987987997: numeric argument required - takes maximum of long long
	9223372036854775807, -9223372036854775808
*/

/* check if string is numeric only. Returns 1 if numeric, 0 if not */
int	str_isnum(char *str)
{
	int	i;

	if (!ft_isdigit(str[0]) && str[0] != '-' && str[0] != '+')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* check value of num. Max exit can take is llong max and llong min */
long long	check_exit_val(const char *str, char *line, t_data *data)
{

	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i])
	{
		if (num > ULLONG_MAX / 10)
		{
			put_strs_fd(3, data, 2, "bash: exit: ", str, ": numeric argument required\n");
			return (255); //it's seriously not 255????
		}
		num = (num * 10) + (str[i] - 48);
		i++;
	}
	if ((neg == 1 && num > LLONG_MAX) || (neg == -1 && num > ((unsigned long long)(LLONG_MAX + 1))))
		{
			put_strs_fd(3, data, 2, "bash: exit: ", str, ": numeric argument required\n");
			// free_data(data, line, YES);
			// close(data->defin);
			// close(data->defout);
			exit (255); //it's seriously not 255????
		}
	// if (neg == -1)
	// 	return (256-(num % 256));
	// return (num % 256);
	// printf("num is : %lli\n", num);
	// printf("llmax: %lli\n", LLONG_MAX);
	return (num * neg);
}

int	ft_exit(t_cmd *cmd,  char *line, t_data *data)
{
	char	**arg;
	int		i;

	arg = cmd->array_arg;
	/* if exit and no other args*/
	if (!arg[1])
	{
		i = 0;
		put_strs_fd(1, data, 1, "exit\n");
	}
	/* if exit and next arg is not a num. exit checks for nonnumbers first */
	else if (!str_isnum(arg[1]))
	{
		i = 255; //check this again bc I thought it was 255
		put_strs_fd(3, data, 2, "bash: exit: ", arg[1], ": numeric argument required\n");
	}
	/* if exit and there are multiple args (so arg[2] exists) - does not exit out of shell */
	else if (arg[2])
	{
		i = 1;
		put_strs_fd(1, data, 2, "bash: exit: too many arguments\n");
		data->num_prev_error = 1;
		return (1);
	}
	/* exit normally */
	else
	{
		i = check_exit_val(arg[1], line, data);
		put_strs_fd(1, data, 1, "exit\n");
	}
	free_data(data, line, YES); // NEED TO EDIT
	close(data->defin);
	close(data->defout);
	exit(i);
	return (0);
}

