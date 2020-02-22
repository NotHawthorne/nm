# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alkozma <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/13 18:06:36 by alkozma           #+#    #+#              #
#    Updated: 2020/02/22 00:29:29 by alkozma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = nm

SRCS = file.c \
	   main.c

OBJECTS = $(patsubst %.c,%.o,$(SRCS))

HEADERS = nm.h

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	gcc $(SRCS) -Iincludes $(FLAGS) -o $(NAME)

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
