#include "../minishell.h"

void	ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		error(data);
	print_string(2, data, cwd, "\n");
	free(cwd);
}
