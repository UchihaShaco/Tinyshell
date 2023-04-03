# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jalwahei <jalwahei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/22 12:18:02 by jalwahei          #+#    #+#              #
#    Updated: 2023/04/01 07:11:51 by jalwahei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	= \
	ft_is_functions.c ft_strl_split.c get_next_line.c \
	ft_join_calloc.c ft_strf.c ft_mems.c ft_itoa_atoi.c ft_strchr.c \
 	ft_itermap_tolu.c ft_put_fd.c ft_lsts.c ft_lstdel_clear.c ft_lstmap.c \

OBJS = $(SRCS:.c=.o)

CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror

NAME			= libft.a

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "\033[35m \033[0m"

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
