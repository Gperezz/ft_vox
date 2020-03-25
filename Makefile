# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gperez <gperez@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/06 13:36:11 by gperez            #+#    #+#              #
#    Updated: 2020/03/25 19:08:42 by gperez           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_vox

FLAGCPP = -std=c++17

FLAG = -Wall -Werror -Wextra

FLAG_OPENGL = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

APP = -framework AppKit

SRC =	srcs/main.cc \
		srcs/Class/Block.cc \
		srcs/Class/Chunck.cc \
		srcs/Class/Engine.cc \
		srcs/Class/Shader.cc \

NC = \033[0m
BOLD =\033[1m
DIM =\033[2m
ITALIC =\033[3m
UNDER =\033[4m
BLINK =\033[5m

RS_BO = \033[21m
RS_D = \033[22m
RS_I = \033[23m
RS_U =\033[24m
RS_BL = \033[25m

WHITE = \033[37m
BLUE = \033[38;5;37m
CYAN = \033[38;5;117m
GREEN = \033[38;5;120m
MAGENTA = \033[38;5;135m
RED = \033[38;5;203m

COLOR1 = \033[38;5;75m
COLOR2 = \033[38;5;178m

LIB_L = libs/libft/libft.a

LIB_P = libs/ft_printf/libftprintf.a

LIB_G = libs/glfw_mac/lib-macos/libglfw3.a 

LIBS_H =	libs/libft/includes \
			libs/ft_printf/includes \
			libs/glfw_mac/include/GLFW \
			libs/glad/include/glad \
			libs/glm \
			libs/glm/gtc \
			includes \
			libs/stb \

LIBS = $(addprefix -I,$(LIBS_H))

INC =	includes/ft_vox.hpp \
		includes/Chunck.hpp \
		includes/Block.hpp \
		includes/Shader.hpp \

OBJ = $(SRC:.cc=.o)

all : $(NAME)

$(LIB_L) :
	@make -C libs/libft

$(LIB_P) :
	@make -C libs/ft_printf

$(NAME) : $(LIB_L) $(LIB_P) $(OBJ)
	@gcc $(FLAG) -o srcs/glad.o -c libs/glad/src/glad.c
	@g++ $(FLAG) $(FLAGCPP) $(FLAG_OPENGL) $(LIB_G) srcs/glad.o $^ -o $(NAME)
	@printf "$(BOLD)$(COLOR1)%20s : $(RS_BL)$(RS_BO)$(GREEN)succesfuly made!$(NC)%20s\n" $(NAME)

%.o : %.cc $(INC)
	@printf "$(BOLD)$(COLOR1)%20s : $(RS_BO)$(COLOR2)%20s$(WHITE) ...$(NC)" $(NAME) $(<F)
	@g++ $(FLAG) $(FLAGCPP) $(LIBS) -o $@ -c $<
	@printf "\r"

clean :
	@make -C libs/libft clean
	@make -C libs/ft_printf clean
	@/bin/rm -rf srcs/*.o

fclean : clean
	@/bin/rm -rf $(NAME)
	@/bin/rm -rf $(LIB_L)
	@/bin/rm -rf $(LIB_P)

re : fclean all
