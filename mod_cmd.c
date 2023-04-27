#include "minishell.h"

/*
In this function, char **file and int *redir are potentially modified (if doubles are found)
1. Iterate through file and redir using count_redir
	if double detected, change redir number to -1
		doubles = < and same file name;
				  > or >> with same file name; > will take precedence
2. iterate through redir using count_redir
	count redir without doubles (-1 value)
3. Compare number to int count_redir
	if same number, return ;
	if ! same number, calloc new char **file and int *redir
4. Copy over filenames and redir values. Use redir -1 value as indicator to skip doubles
5. Free old char **file and int *redir
6. Set cmd->file = new_file, cmd->redir = new_redir, cmd->count_redir = new_count;
*/
void	mod_redir_doubles(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (i < cmd->count_redir)
	{
		if (cmd->redir[i] != 5)
		{
			while ((cmd->redir[i] != -1) && j < cmd->count_redir)
			{
				if (ft_strcmp(cmd->file[i], cmd->file[j]) == 0)
				{
					if (cmd->redir[i] == cmd->redir[j] || (cmd->redir[i] == 3 && cmd->redir[j] == 4))
						cmd->redir[j] = -1;
					else if (cmd->redir[i] == 4 && cmd->redir[j] == 3)
						cmd->redir[i] = -1;
				}
				j++;
			}
		}
		i++;
		j = i + 1;
	}
}

/* modifies cmd->file and cmd->redir */
void	check_redir_doubles(t_cmd *cmd, t_data *data)
{
	int		i;
	int		j;
	int		new_count_redir;
	char	**new_file;
	int		*new_redir;

	mod_redir_doubles(cmd);
	i = -1;
	new_count_redir = 0;
	while (++i < cmd->count_redir)
		if (cmd->redir[i] != -1)
			new_count_redir++;
	if (cmd->count_redir == new_count_redir)
		return ;
	new_file = (char **)ts_calloc(new_count_redir + 1, sizeof(char *), data);
	new_redir = (int *)ts_calloc(new_count_redir, sizeof(int), data);
	i = -1;
	j = 0;
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] != -1)
		{
			new_file[j] = ft_strdup_lim(cmd->file[i], '\0', data);
			new_redir[j] = cmd->redir[i];
			j++;
		}
	}
	free_strlist(cmd->file);
	free(cmd->redir);
	cmd->file = new_file;
	cmd->redir = new_redir;
	cmd->count_redir = new_count_redir;
}

/* populates fd_array */
void	open_redir_files(int redir, int i, t_cmd *cmd, t_data *data)
{
	if (redir == 2)
	{
		cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
		if (cmd->fd_array[i] == -1)
		{
			printf("-bash: %s: No such file or directory\n", cmd->file[i]);
			//close fd_array, send null to pipe, and exit out of this child process to the next one
		}
	}
	else if (redir == 3)
	{
		cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
		if (cmd->fd_array[i] == -1)
			error(ERR_OPEN, data);
	}
	else if (redir == 4)
	{
		cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
		if (cmd->fd_array[i] == -1)
			error(ERR_OPEN, data);
	}
	else if (redir == 5)
		cmd->fd_array[i] = -1;
}

/* modifies last_input, last_output, count_hd, init fd_array*/
void	mod_fd_array(t_cmd *cmd, t_data *data)
{
	int	i;

	cmd->last_output = -1;
	cmd->last_input = -1;
	cmd->fd_array = ts_calloc(cmd->count_redir, sizeof(int), data);
	i = -1;
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] == 2)
		{
			open_redir_files(2, i, cmd, data);
			cmd->last_input = i;
		}
		else if (cmd->redir[i] == 3)
		{
			open_redir_files(3, i, cmd, data);
			cmd->last_output = i;
		}
		else if (cmd->redir[i] == 4)
		{
			open_redir_files(4, i, cmd, data);
			cmd->last_output = i;
		}
		else if (cmd->redir[i] == 5)
		{
			open_redir_files(5, i, cmd, data);
			cmd->last_input = i;
			cmd->count_hd++;
		}
	}
}

/* modifies hd_array, record_hd, fd_array (if last input is hd); inits hd in terminal */
void	create_heredoc(t_cmd *cmd, t_data *data)
{
	int	i;
	int	j;

	cmd->hd_array = ts_calloc(cmd->count_hd + 1, sizeof(char *), data);
	i = -1;
	j = -1;
	while (++i < cmd->count_redir)
		if (cmd->redir[i] == 5)
			cmd->hd_array[++j] = ft_strdup_lim(cmd->file[i], '\0', data);
	if (cmd->last_input > -1 && cmd->redir[cmd->last_input] == 5)
		cmd->record_hd = 1;
	get_heredoc(cmd, data);
}

void	get_cmd_path(t_cmd *cmd, t_data *data)
{
	int		i;
	char	*arg;

	i = 0;
	arg = cmd->array_arg[0];
	if ((arg, F_OK) == 0 && access(arg, X_OK) == 0)
	{
		cmd->path = ft_strdup_lim(arg, '\0', data);
		return ;
	}
	if (!data->env_paths)
		return ;
	while (data->env_paths[i])
	{
		cmd->path = ft_strjoin_char(data->env_paths[i], arg, '/', data);
		if (access(cmd->path, F_OK) == 0 && access(cmd->path, X_OK) == 0)
			return ;
		else if (access(cmd->path, F_OK) == 0 && access(cmd->path, X_OK) < 0)
		{
			free(cmd->path);
			cmd->path = NULL;
			return ;
		}
		free(cmd->path);
		cmd->path = NULL;
		i++;
	}
}

void	finalize_cmd(t_data *data)
{
	int	i;

	i = -1;
	while(++i < data->num_cmd)
	{
		// printf("%i\n", data->cmd[i].count_redir);
		if (data->cmd[i].count_redir > 1) //only check for doubles if there are 2+ redir
			check_redir_doubles(&data->cmd[i], data);
		mod_fd_array(&data->cmd[i], data);
		if (data->cmd[i].count_hd > 0)
			create_heredoc(&data->cmd[i], data);
		if(data->cmd->num_arg > 0)
			get_cmd_path(&data->cmd[i], data);
		data->cmd[i].builtin = check_builtin(&data->cmd[i], data);
	}
}