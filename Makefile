# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/09 11:48:59 by jalwahei          #+#    #+#              #
#    Updated: 2023/04/05 04:37:58 by jalwahei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
SRCS =	minishell.c parse/ts_signals.c parse/ts_malloc.c parse/ts_count_record_cmd.c parse/parse_pipe.c parse/ts_find_redirect.c parse/ts_error.c \
	parse/ts_free.c parse/ts_record_val.c ts_utils.c parse/ts_quotation_marks.c parse/ts_record_arr.c parse/ts_measure_size_file_name.c parse/ts_init_env.c \
	parse/ts_found_dollar.c utils.c \

CC = cc

CFLAGS = -I. -g -Wall -Wextra

OBJS = $(SRCS:.c=.o)

all: $(NAME)

lib = libft/libft.a

$(NAME): $(OBJS)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJS) $(lib) -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -l readline -o $(NAME)
	@echo "\033[35mTinyshell Ready\033[0m"

clean:
	@rm -f $(OBJS)
	@make clean -C libft

fclean: clean
	@rm -f $(NAME)
	@make fclean -C libft

re: fclean all
