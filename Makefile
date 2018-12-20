# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acottier <acottier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/01 13:55:41 by acottier          #+#    #+#              #
#    Updated: 2018/12/19 13:01:26 by acottier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED			=\033[0;31m
GREEN		=\033[0;32m
YELLOW		=\033[0;33m
NC			=\033[0m

TARGETS		=	ft_nm \
				ft_otool

LIB			= libft

.PHONY: all clean fclen re fast

all : $(NAME)

$(NAME) :
	@make -C $(LIB)
	@make -C ft_nm
	# @make -C ft_otool

clean :
	@make clean -C $(LIB)
	@make clean -C ft_nm
	# @make clean -C ft_otool

fclean :
	@make fclean -C $(LIB)
	@make fclean -C ft_nm
	# @make fclean -C ft_otool

re : fclean all
	@make re -C $(LIB)
	@make re -C ft_nm
	# @make re -C ft_otool