#include "../minishell.h"

/* bash can take any case of builtin command. ex: echo vs eCHo both work */
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

/* check first str in array_arg to see if it's a builtin and assign num id */
int	check_builtin(t_cmd *cmd, t_data *data)
{
	char	*str;
	char	*command;
	int		i;

	str = cmd->array_arg[0];
	command = lower_str(str, data);
	i = 0;
	if (ft_strcmp(command, "echo") == 0)
		i = 1;
	else if (ft_strcmp(command, "cd") == 0)
		i = 2;
	else if (ft_strcmp(command, "pwd") == 0)
		i = 3;
	else if (ft_strcmp(command, "export") == 0)
		i = 4;
	else if (ft_strcmp(command, "unset") == 0)
		i = 5;
	else if (ft_strcmp(command, "env") == 0)
		i = 6;
	else if (ft_strcmp(command, "exit") == 0)
		i = 7;
	free(command);
	return (i);
}

int	execute_builtin(t_cmd *cmd, int	proc, char *line, t_data *data)
{
	char	**arg;
	int		exit_val;

	arg = cmd->array_arg;
	if (cmd->builtin == 1)
		exit_val = ft_echo(cmd, data);
	else if (cmd->builtin == 2)
		exit_val = ft_cd(cmd, data);
	else if (cmd->builtin == 3)
		exit_val = ft_pwd(data);
	else if (cmd->builtin == 4)
		exit_val = ft_export(cmd, data);
	else if (cmd->builtin == 5)
		exit_val = ft_unset(cmd, data);
	else if (cmd->builtin == 6)
		exit_val = ft_env(data);
	else if (cmd->builtin == 7)
		ft_exit(cmd, line, data);
	/* if a child process exit with exit value */
	if (proc == CHILD)
		exit(exit_val);
	/* if parent, return exit value */
	return (exit_val);

}