# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/09 11:48:59 by jalwahei          #+#    #+#              #
#    Updated: 2023/05/07 13:58:37 by jalwahei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCS =	minishell.c parse/ts_signals.c parse/ts_malloc.c parse/ts_count_record_cmd.c parse/parse_pipe.c \
	parse/ts_find_redirect.c parse/ts_error.c \
	parse/ts_record_val.c utils/ts_utils.c parse/ts_quotation_marks.c parse/ts_record_arr.c \
	parse/ts_measure_size_file_name.c parse/ts_init_env.c parse/ts_found_dollar.c utils/utils.c utils/put_str.c  \
	utils/utils_h.c srcs/mod_cmd.c srcs/mod_cmd_utils.c utils/free.c srcs/heredoc.c srcs/data.c srcs/execution3_utils2.c srcs/execution3.c srcs/execution3_utils.c\
	builtin/echo.c builtin/env.c builtin/export_utils.c builtin/export.c builtin/builtin.c \
	builtin/pwd.c builtin/unset.c builtin/cd.c builtin/exit.c builtin/export_utils2.c\
	env/ourenv.c env/envlist.c env/mod_envlist.c srcs/heredoc2.c srcs/execution_utils3.c

CC = cc

CFLAGS = -I. -Wall -Werror -Wextra

OBJS = $(SRCS:.c=.o)

all: $(NAME)

lib = libft/libft.a

$(NAME): $(OBJS)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJS) $(lib) -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -l readline -o $(NAME)
	@echo "\033[35mTinyShell Ready\033[0m"

clean:
	@rm -f $(OBJS)
	@make clean -C libft

fclean: clean
	@rm -f $(NAME)
	@make fclean -C libft

re: fclean all
