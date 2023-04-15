#include "../minishell.h"

void	ft_echo(char **arg, t_data *data)
{
	int	i;
	int	nl;

	//check for -n flag. if there is, start the printing at index 2, else start at index 1
	if (arg[1] && ft_strncmp(arg[1], "-n", 3) == 0) 
	{
		i = 2;
		nl = 0;
	}
	else
	{
		i = 1;
		nl = 1;
	}
	//loop through args and print
	while (arg[i])
	{
		if (arg[i + 1] != NULL)
			print_string(2, data, arg[i], " ");
		else if (arg[i + 1] == NULL)
			print_string(1, data, arg[i]);
		i++;
	}
	//if no -n, also print /n after the last string is printed
	if (nl == 1)
		print_string(1, data, "\n");
}
