#include "../minishell.h"

int	ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		error(ERR_CWD, data);
	print_string(2, data, cwd, "\n");
	free(cwd);
	return (0);
}
