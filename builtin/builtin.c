#include "../minishell.h"

char	*lower_str(char *str, t_data *data)
{
	int		i;
	char	*low_str;

	i = 0;
	low_str = (char *)ts_calloc(ft_strlen(str) + 1, sizeof(char), data);
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			low_str[i] = str[i] + 32;
		else
			low_str[i] = str[i];
		i++;
	}
	return (low_str);
}

int	check_builtin(char *str, t_data *data)
{
	char	*command;
	int		i;

	command = lower_str(str, data);
	i = 0;
	if (ft_strncmp(command, "echo", ft_strlen(command)) == 0)
		i = 1;
	else if (ft_strncmp(command, "cd", ft_strlen(command)) == 0)
		i = 2;
	else if (ft_strncmp(command, "pwd", ft_strlen(command)) == 0)
		i = 3;
	else if (ft_strncmp(command, "export", ft_strlen(command)) == 0)
		i = 4;
	else if (ft_strncmp(command, "unset", ft_strlen(command)) == 0)
		i = 5;
	else if (ft_strncmp(command, "env", ft_strlen(command)) == 0)
		i = 6;
	else if (ft_strncmp(command, "exit", ft_strlen(command)) == 0)
		i = 7;
	free(command);
	return (i);
}

void	execute_builtin(char **arg, int	i, t_data *data)
{
	if (i == 1)
		ft_echo(arg, data);
	// else if (i == 2)
	// 	ft_cd(arg, data);
	// else if (i == 3)
	// 	ft_pwd(data);
	else if (i == 4)
	{
		ft_export(arg, data);
	}
	// else if (i == 5)
	// 	ft_unset(arg, data);
	else if (i == 6)
		ft_env(data);
	// else if (i == 7)
	// 	ft_exit(data);
	// exit(0);./
}