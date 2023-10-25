# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wayden <wayden@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/14 12:12:31 by wayden            #+#    #+#              #
#    Updated: 2023/10/25 18:27:17 by wayden           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -pthread -Wall -Wextra -Werror -g3 

FILES = main utils singletons error_manager time philosophers_life mutex

DIR = ./src/

SRCS   = $(addprefix $(DIR), $(addsuffix .c, $(FILES)))
OBJ	   = $(addprefix $(DIR), $(addsuffix .o, $(FILES)))
OBJ_BONUS = $(addprefix $(DIR), $(addsuffix .o, $(BONUS_FILES)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ${OBJ}
	
fclean: clean
	rm -rf $(NAME)
	
re: fclean all
.PHONY: