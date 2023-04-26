#include "../minishell.h"

void	ft_env(t_data *data)
{
	char	**our_env;
	int		i;

	our_env = data->our_env;
	i = 0;
	while(our_env[i])
	{
		print_string(2, data, our_env[i], "\n");
		i++;
	}
}