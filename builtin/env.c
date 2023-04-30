#include "../minishell.h"

/* print env using our_env */

int	ft_env(t_data *data)
{
	char	**our_env;
	int		i;

	our_env = data->our_env;
	if (!our_env)
		return (0);
	i = 0;
	while(our_env[i])
	{
		put_strs_fd(2, data, 1, our_env[i], "\n");
		i++;
	}
	return (0);
}