# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/09 11:48:59 by jalwahei          #+#    #+#              #
#    Updated: 2023/03/10 16:34:23 by jalwahei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =	minishell.c ts_signals.c ts_malloc.c ts_count_record_cmd.c parse_pipe.c ts_find_redirect.c ts_error.c ts_free.c ts_utils.c ts_record_val.c ts_quotation_marks.c ts_record_arr.c ts_measure_size_file_name.c

CC = cc

# CFLAGS =  -Wall -Wextra -Werror  -g -fsanitize=address


OBJS = $(SRCS:.c=.o)

all: $(NAME)
lib = libft/libft.a

$(NAME): $(OBJS)
	make -C libft
	$(CC) $(OBJS) $(lib) -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline -o $(NAME)

clean:
	rm -f $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

