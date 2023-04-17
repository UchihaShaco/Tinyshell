#include "minishell.h"

/* initiate array of fd and pid in data */
void	init_fd_pid(t_data *data)
{
	int	i;

	data->fd = (int **)ft_calloc_e(data->num_cmd - 1, sizeof(int *), data);
	i = 0;
	while (i < data->num_cmd - 1)
	{
		data->fd[i] = (int *)ft_calloc_e(2, sizeof(int), data);
		i++;
	}
	data->pid = (int *)ft_calloc_e(data->num_cmd, sizeof(int), data);
}

/* initiate linked list of env variables in data */
void	init_env(t_data *data, char **envp)
{
	t_env	**env_list;
	t_env	*node;
	t_env	*cur;
	char	**split_var;
	int		i;

	i = -1;
	env_list = (t_env **)ft_calloc_e(1, sizeof(t_env *), data);
	cur = NULL;
	while (envp[++i])
	{
		node = (t_env *)ft_calloc_e(1, sizeof(t_env), data);
		split_var = split_env_var(envp[i], data);
		node->key = ft_strdup_lim(split_var[0], '\0', data);
		node->val = ft_strdup_lim(split_var[1], '\0', data);
		node->next = NULL;
		node->prev = cur;
		free_strlist(split_var);
		if (!*env_list)
			*env_list = node;
		else
			cur->next = node;
		cur = node;
	}
	data->env_list = env_list;
}

/* initialize data structure */
void	ts_init_data(t_data *data, char ***env, int first)
{
	//NOTES: if we calloc the struct, we can initiate eveyrthing to 0 and NULL
	if (first == YES)
	{
		data->cur_dir = getcwd(NULL, 0);
		data->num_prev_error = 0; // check header :P
		data->num_error = 0;
		ts_init_env(data, env);
		data->name_file = NO; // flag to check if it's a file (YES, NO)
		init_env(data, *env); //init linked list env variables
		get_env_paths(*env, data); //list of environmental paths (for execve)
	}
	data->num_prev_error = data->num_error;
	data->num_error = 0;
	data->empty_str = NO; // flag to check if the string is empty (YES, NO)
	data->home_dir = getenv("HOME");
	data->num_cmd = 0;
}