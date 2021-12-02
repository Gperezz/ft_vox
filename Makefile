# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/10 18:22:58 by gperez            #+#    #+#              #
#    Updated: 2021/12/02 10:03:10 by maiwenn          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = ft_vox

FLAGCPP = -std=c++11

FLAG = -Wall -g -O2 

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
FLAG_OPENGL =	-lGL	\
				-lpthread
endif
ifeq ($(UNAME), Darwin)
FLAG_OPENGL =	-framework Cocoa	\
				-framework OpenGL	\
				-framework IOKit	\
				-framework CoreVideo	
endif

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
		srcs/Class/Cave.cc \
		srcs/Class/Error.cc \

ifeq ($(UNAME), Linux)
LIB_GLFW = libs/glfw/src/libglfw.so.3.4

LIB_GLAD = libs/glad/libglad.so
endif
ifeq ($(UNAME), Darwin)
LIB_GLFW = libs/glfw/src/libglfw.3.4.dylib

LIB_GLAD = libs/glad/libglad.dylib
endif
LIBS_H =	libs/ \
			includes \
			libs/glad/include/ \
			libs/glfw/include/GLFW/\
			
LIB_GLM =	libs/glm/glm/gtc/ \
			libs/glm/glm 

LIB_STB = libs/stb/include

LIBS = $(addprefix -I,$(LIBS_H) $(LIB_STB) $(LIB_GLM))

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
		includes/Engine.hpp \
		includes/Error.hpp \

OBJ = $(SRC:.cc=.o)

.PHONY : all
all :  $(NAME) 

$(OBJ): $(LIB_GLM) $(LIB_GLAD) $(LIB_STB) $(LIB_GLFW)

ifeq ($(UNAME), Linux)
$(NAME) : $(OBJ)
	g++ $^ -Wl,-rpath=$(PWD)/libs/glad/ -Wl,-rpath=$(PWD)/libs/glfw/src/ $(FLAG) $(FLAGCPP) $(FLAG_OPENGL) $(LIB_GLAD) $(LIB_GLFW)  -o $(NAME)
endif
ifeq ($(UNAME), Darwin)
$(NAME) : $(OBJ)
	g++ $(FLAG) $(FLAGCPP) $(FLAG_OPENGL) $(LIB_GLAD) $(LIB_GLFW) $^ -o $(NAME)
	install_name_tool -add_rpath @executable_path/libs/glad/ $(NAME)
	install_name_tool -change /usr/local/lib/libglad.dylib @rpath/libglad.dylib $(NAME)
	install_name_tool -add_rpath @executable_path/libs/glfw/src/ $(NAME)
	install_name_tool -change /usr/local/lib/libglfw.3.4.dylib @rpath/libglfw.3.4.dylib $(NAME)
endif

libs/glm/CMakeLists.txt :
	git clone  https://github.com/g-truc/glm.git libs/glm --depth=1

$(LIB_GLM) : libs/glm/CMakeLists.txt

libs/glad/CMakeLists.txt :
	git clone https://github.com/Dav1dde/glad.git libs/glad --depth=1

$(LIB_GLAD) : libs/glad/CMakeLists.txt
	cmake libs/glad/CMakeLists.txt -D BUILD_SHARED_LIBS=ON -D GLAD_REPRODUCIBLE=ON
	cmake --build libs/glad/.

libs/stb/include/stb_image.h :
	git clone https://github.com/franko/stb_image.git libs/stb --depth=1

$(LIB_STB) : libs/stb/include/stb_image.h

libs/glfw/CMakeLists.txt :
	git clone https://github.com/glfw/glfw.git libs/glfw --depth=1

$(LIB_GLFW) : libs/glfw/CMakeLists.txt
	cmake libs/glfw/CMakeLists.txt -D BUILD_SHARED_LIBS=ON -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF
	cmake --build libs/glfw/.


%.o : %.cc $(INC)
	g++ $(FLAG) $(FLAGCPP) $(LIBS) -o $@ -c $<

.PHONY : clean
clean :
	rm -rf srcs/*.o
	rm -rf srcs/Class/*.o

.PHONY : fclean
fclean : clean
	rm -rf $(NAME)

.PHONY : lclean
lclean : fclean
	rm -rf libs/glad
	rm -rf libs/glfw
	rm -rf libs/glm
	rm -rf libs/stb

.PHONY : re
re : fclean all

.PHONY : relib
relib : lclean all