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
		while (cmd->redir[i] != -1 && j < cmd->count_redir)
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
		i++;
		j = i + 1;
	}
}

void	check_redir_doubles(t_cmd *cmd)
{
	int		i;
	int		new_count_redir;
	char	*new_file;
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
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] != -1)
		{
			new_file[i] = ft_strdup_lim(cmd->file[i]);
			new_redir[i] = cmd->redir[i];
		}
	}
	free_strlist(cmd->file);
	free(cmd->redir);
	cmd->file = new_file;
	cmd->redir = new_redir;
}

void	finalize_cmd(t_data *data)
{
	int	i;

	i = 0;
	while(data->cmd[i])
	{
		if (data->count_redir > 1) //only check for doubles if 2+ redirects
			check_redir_doubles(&data->cmd[i]);
	}

}