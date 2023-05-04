/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ts_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 03:29:53 by jalwahei          #+#    #+#             */
/*   Updated: 2023/05/04 07:36:46 by jalwahei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ts_record_char(char **result, char *str, int *r, int *s)
{
	(*result)[(*r)] = str[(*s)];
	(*r)++;
	(*s)++;
}

void	ts_record_str(char **file, char *str, int start, int size_str)
{
	int	i;

	i = 0;
	while (i < size_str)
	{
		(*file)[i] = str[start + i];
		i++;
	}
	(*file)[i] = '\0';
}

int	ts_cut_qm_in_name_file(char **file, t_data *data)
{
	int		i;
	char	qm;

	i = 0;
	qm = 0;
	while ((*file)[i] != '\0')
	{
		qm = (*file)[i];
		if (qm == 34 || qm == 39)
		{
			ts_replace_key_to_value(file, 1, NULL, i);
			while ((*file)[i] != qm && (*file)[i] != '\0')
				i++;
			if ((*file)[i] != '\0')
				ts_replace_key_to_value(file, 1, NULL, i);
		}
		else
			i++;
	}
	return (0);
}

void	free_util(t_data *data, char *line)
{
	free_cmd(data);
	free_fdlist(data);
	if (data->pid)
	{
		free(data->pid);
		data->pid = NULL;
	}
	if (line)
	{
		free(line);
		line = NULL;
	}
}

char	*ft_strdup_start(const char *s, int start)
{
	int		i;
	char	*str1;

	if (s == NULL)
		return (NULL);
	if ((size_t)start >= ft_strlen(s) || start < 0)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0')
		i++;
	str1 = (char *)malloc(sizeof(char) * (i + 1));
	if (str1 == NULL)
		return (NULL);
	i = 0;
	while (s[start + i] != '\0')
	{
		str1[i] = s[start + i];
		i++;
	}
	str1[i] = '\0';
	return (str1);
}

/* void print_t_data(struct s_data data) 
{
    printf("t_data {\n");
	int k=0;
		while(data.cmd[k].str != NULL)
    	{printf("    t_cmd *cmd: %s\n", data.cmd[k].str);
	k++;}
    // printf("    t_tmp tmp: %d\n", data.tmp);
    // printf("    int num_cmd: %d\n", data.num_cmd);
    // printf("    int num_error: %d\n", data.num_error);
    // printf("    int num_prev_error: %d\n", data.num_prev_error);
    // printf("    int num_env: %d\n", data.num_env);

    // Print 2D array `our_env`
    // printf("    char **our_env: {\n");
    // for (int i = 0; i < data.num_env; i++) {
    //     printf("        \"%s\",\n", data.our_env[i]);
    // }
    // printf("    }\n");

    // Print contents of 2D array `tmp_var`
    // printf("    char **tmp_var: {\n");
    // for (int i = 0; i < data.num_tmp_var; i++) {
    //     printf("        \"%s\",\n", data.tmp_var[i]);
    // }
    // printf("    }\n");

    // printf("    int num_tmp_var: %d\n", data.num_tmp_var);
    // printf("    char *prev_dir: %p\n", data.prev_dir);
    // // printf("    char *cur_dir: %p\n", data.cur_dir);
    // // printf("    char *home_dir: %p\n", data.home_dir);
    // printf("    int flag_old: %d\n", data.flag_old);
    // printf("    int empty_str: %d\n", data.empty_str);
    // printf("    int fd_pipe[2]: {%d, %d}\n", data.fd_pipe[0], data.fd_pipe[1]);
    // printf("    int name_file: %d\n", data.name_file);
    // printf("    int build_in: %d\n", data.build_in);
    // printf("    int *pid: %p\n", data.pid);
    // printf("    int n_end: %d\n", data.n_end);

    // // Print string `prev_dir` if not NULL
    // if (data.prev_dir != NULL) {
    //     printf("    prev_dir: \"%s\"\n", data.prev_dir);
    // }

    // // Print string `cur_dir` if not NULL
    // if (data.cur_dir != NULL) {
    //     printf("    cur_dir: \"%s\"\n", data.cur_dir);
    // }

    // // Print string `home_dir` if not NULL
    // if (data.home_dir != NULL) {
    //     printf("    home_dir: \"%s\"\n", data.home_dir);
    // }

    printf("}\n");
}*/