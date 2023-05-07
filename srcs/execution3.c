/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 05:12:31 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 05:12:31 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(int i, t_cmd *cmd, char *line, t_data *data)
{
	close(data->defin);
	close(data->defout);
	open_files(cmd, CHILD, data);
	pipe_cmd(i, cmd, data);
	close_fd_array(cmd, data);
	close_pipes(data);
	if (cmd->num_arg == 0)
		exit(0);
	if (cmd->builtin > 0)
		execute_builtin(cmd, CHILD, line, data);
	check_as_dir(cmd, data);
	check_as_command(cmd, data);
}

int	exec_one_builtin(t_cmd *cmd, char *line, t_data *data)
{
	int	status;

	if (open_files(cmd, PARENT, data) == 1)
		return (1);
	if (cmd->last_input > -1)
	{
		ts_dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO, data);
	}
	if (cmd->last_output > -1)
	{
		ts_dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO, data);
	}
	close_fd_array(cmd, data);
	status = execute_builtin(&data->cmd[0], PARENT, line, data);
	return (status);
}

int	parent_process(t_data *data)
{
	int	i;
	int	status;

	close_pipes(data);
	i = 0;
	while (i < data->num_cmd)
	{
		waitpid(data->pid[i], &status, 0);
		i++;
	}
	return (status);
}

int	run_command(char *line, t_data *data)
{
	int	i;
	int	status;

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
		{
			signal(SIGQUIT, qhandler);
			child_process(i, &data->cmd[i], line, data);
		}
	}
	status = parent_process(data);
	return (status);
}

/* set exit status here */
void	execute(char *line, t_data *data)
{
	int	status;

	if (data->num_cmd == 0)
		data->num_error = 0;
	else if (data->num_cmd == 1 && data->cmd->builtin > 0)
		data->num_error = exec_one_builtin(&data->cmd[0], line, data);
	else
	{
		status = run_command(line, data);
		data->num_error = WEXITSTATUS(status);
	}
}
