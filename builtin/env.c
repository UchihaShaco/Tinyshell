#include "../minishell.h"

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
		print_string(2, data, our_env[i], "\n");
		i++;
	}
	return (0);
}