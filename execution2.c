#include "minishell.h"


/* proc is either CHILD or PARENT */
int	open_files(t_cmd *cmd, int proc, t_data *data)
{
	int	i;

	if (cmd->count_redir == 0)
		return (0);
	cmd->fd_array = (int *)ts_calloc(cmd->count_redir, sizeof(int), data);
	i = -1;
	while (++i < cmd->count_redir)
	{
		if (cmd->redir[i] == 2)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_RDONLY);
			if (cmd->fd_array[i] == -1)
			{
				put_strs_fd(3, data, 2, "bash: ", cmd->file[i], ": No such file or directory\n");
				close_fd_array(cmd, data);
				close_pipes(data);
				if (one_func == NO)
					exit (EXIT_FAILURE);
				else
				{
					close(data->defin);
					close(data->defout);
					return (1);
				}
			}
		}
		else if (cmd->redir[i] == 3)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
			if (cmd->fd_array[i] == -1)
				error(ERR_OPEN, data);
		}
		else if (cmd->redir[i] == 4)
		{
			cmd->fd_array[i] = open(cmd->file[i], O_CREAT | O_RDWR | O_APPEND, 0666);
			if (cmd->fd_array[i] == -1)
				error(ERR_OPEN, data);
		}
		else if (cmd->redir[i] == 5)
		{
			if (cmd->last_input == i)
				get_heredoc_fd(cmd, data);
			else 
				cmd->fd_array[i] = -2;
		}
	}
	return(0);
}

/* set exit status here */
void	execute(char *line, t_data *data)
{
	int	i;
	int	status;

	if (data->num_cmd == 0)
		data->num_prev_error = 0;
	else if (data->num_cmd == 1 && data->cmd->builtin > 0)
		data->num_prev_error =  exec_one_builtin(&data->cmd[0], line, data);
	else
	{
		i = -1;
		while (++i < data->num_cmd - 1)
			if (pipe(data->fd[i]) == -1)
				error(ERR_PIPE, data);
		i = -1;
		while (++i < data->num_cmd)
		{
			data->pid[i] = fork();
			if (data->pid[i] == -1)
				error(ERR_FORK, data);
			else if (data->pid[i] == 0)
				child_process(i, &data->cmd[i], line, data);
		}
		data->num_error = WEXITSTATUS(parent_process(data));
	}
}
