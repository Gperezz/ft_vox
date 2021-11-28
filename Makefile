# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/10 18:22:58 by gperez            #+#    #+#              #
#    Updated: 2021/11/28 14:47:19 by maiwenn          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_vox

FLAGCPP = -std=c++11

FLAG = -Wall -g -O2 #-flto # -Werror -Wextra

FLAG_OPENGL = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

APP = -framework AppKit

SRC =	srcs/main.cc \
		srcs/Class/Engine.cc \
		srcs/Class/Shader.cc \
		srcs/Class/Textures.cc \
		srcs/Class/Camera.cc \
		srcs/Class/Mat.cc \
		srcs/Class/World.cc \
		srcs/Class/Chunk.cc \
		srcs/Class/Block.cc \
		srcs/Class/WorldGenerator.cc \
		srcs/Class/TimeMs.cc \
		srcs/Class/Hud.cc \
		srcs/Class/Element.cc \
		srcs/Class/Perlin.cc \
		srcs/Class/Cave.cc

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

LIB_G = libs/glfw_mac/lib-macos/libglfw3.a 

LIBS_H =	libs/glfw_mac/include/GLFW \
			libs/ \
			includes \
			libs/glad/include/ \
			

LIB_GLM =	libs/glm/glm \
			libs/glm/glm/gtc

LIB_GLAD = libs/glad/libglad.dylib

LIB_STB = libs/stb/include

LIBS = $(addprefix -I,$(LIBS_H) $(LIB_GLM) $(LIB_STB))

INC =	includes/ft_vox.hpp \
		includes/Coords.hpp \
		includes/Chunk.hpp \
		includes/TimeMs.hpp \
		includes/StructBlock.hpp \
		includes/Textures.hpp \
		includes/World.hpp \
		includes/Engine.hpp \
		includes/Perlin.hpp \
		includes/Cave.hpp \
		includes/Camera.hpp \

OBJ = $(SRC:.cc=.o)

.PHONY : all
all :  $(NAME) 

$(OBJ): $(LIB_GLM) $(LIB_GLAD) $(LIB_STB)

$(NAME) : $(OBJ) 
	g++ $(FLAG) $(FLAGCPP) $(FLAG_OPENCL) $(FLAG_OPENGL) $(LIB_G) $(LIB_GLAD) $^ -o $(NAME)
	install_name_tool -add_rpath @executable_path/libs/glad/ $(NAME)
	install_name_tool -change /usr/local/lib/libglad.dylib @rpath/libglad.dylib $(NAME)


libs/glm/CMakeLists.txt :
	git clone https://github.com/g-truc/glm.git libs/glm

$(LIB_GLM) : libs/glm/CMakeLists.txt
	cmake libs/glm/CMakeLists.txt -D BUILD_SHARED_LIBS=ON
	cmake --build libs/glm/.  #build faster

libs/glad/CMakeLists.txt :
	git clone https://github.com/Dav1dde/glad.git libs/glad

$(LIB_GLAD) : libs/glad/CMakeLists.txt
	cmake libs/glad/CMakeLists.txt -D BUILD_SHARED_LIBS=ON
	cmake --build libs/glad/.  #build faster

libs/stb/include/stb_image.h :
	git clone https://github.com/franko/stb_image.git libs/stb

$(LIB_STB) : libs/stb/include/stb_image.h


%.o : %.cc $(INC)
	g++ $(FLAG) $(FLAGCPP) $(LIBS) -o $@ -c $<

.PHONY : clean
clean :
	@/bin/rm -rf srcs/*.o
	@/bin/rm -rf srcs/Class/*.o

.PHONY : fclean
fclean : clean
	@/bin/rm -rf $(NAME)

.PHONY : re
re : fclean all