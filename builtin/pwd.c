#include "../minishell.h"

int	ft_pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		error(ERR_CWD, data);
	put_strs_fd(2, data, 1, cwd, "\n");
	free(cwd);
	return (0);
}
