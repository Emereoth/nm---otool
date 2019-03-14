# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acottier <acottier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/01 13:55:41 by acottier          #+#    #+#              #
#    Updated: 2019/03/14 18:03:03 by acottier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED			=	\033[0;31m
GREEN		=	\033[0;32m
YELLOW		=	\033[0;33m
NC			=	\033[0m

NAME		=	ft_otool

LIB_PATH	=	../libft
LIB			=	../libft/libft.a

LOG			=	log

SRC_PATH	=	sources
OBJ_PATH	=	objects

SRC_NAMES	=	main.c \
				bin32.c \
				bin64.c \
				data.c \
				endian_swaps.c \
				errors.c \
				utilities.c \
				static_lib.c \
				archive_list.c \
				stringtab_check.c \
				check_bounds.c

OBJ_NAMES	=	$(SRC_NAMES:.c=.o)

SRC			=	$(addprefix $(SRC_PATH)/, $(SRC_NAMES))
OBJ			=	$(addprefix $(OBJ_PATH)/, $(OBJ_NAMES))

CC			=	gcc
FLAGS		=	-Wall -Wextra -Werror

.PHONY: all clean fclean re fast debug

all : lib $(NAME)

lib:
	@make re -C $(LIB_PATH)
	@make clean -C $(LIB_PATH)

$(NAME) : $(OBJ)
	@(time $(CC) $(FLAGS) $(LIB) $^ -o $@) 2> time.txt
	@printf "${GREEN}'ft_otool' executable compilation finished:${NC} "
	@printf "${YELLOW}"
	@cat time.txt | head -2 | tail -1
	@printf "${NC}"
	@rm time.txt

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	@mkdir -p $(OBJ_PATH) && gcc $(FLAGS) -o $@ -c $<

clean :
	@rm -rf $(OBJ_PATH)
	@printf "${RED}'ft_otool' executable object files deleted.${NC}\n"

fclean : clean
	@@rm -f $(NAME) $(LOG)
	@printf "${RED}'ft_otool' executable deleted.${NC}\n"

re : fclean all

fast : fclean
	$(CC) $(FLAGS) $(SRC) -o $(NAME) $(LIB)

debug: fclean
	$(CC) $(FLAGS) -g3 $(SRC) -o $(NAME) $(LIB)