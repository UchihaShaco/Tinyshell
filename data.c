#include "minishell.h"

char	*find_home_dir(t_data *data)
{
	t_env	*home;
	char	*home_path;

	home = find_var_envlist("HOME", data);
	if (!home)
		//substitute home directory associated with specific username"
		home_path = NULL;
	else
		home_path = home->val;
	return (home_path);
}

/* initiate array of fd and pid in data */
/* NOTE: must be created after data->num_cmd finalized */
void	create_fd_pid_array(t_data *data)
{
	int	i;

	data->fd = (int **)ts_calloc(data->num_cmd - 1, sizeof(int *), data);
	i = 0;
	while (i < data->num_cmd - 1)
	{
		data->fd[i] = (int *)ts_calloc(2, sizeof(int), data);
		i++;
	}
	data->pid = (int *)ts_calloc(data->num_cmd, sizeof(int), data);
}