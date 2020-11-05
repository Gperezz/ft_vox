/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:35:15 by gperez            #+#    #+#             */
/*   Updated: 2020/11/05 12:35:07 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ENGINE_HPP_
# define _ENGINE_HPP_

extern "C"
{
	# include "ft_printf.h"
	# include "glad.h" // Implementation OpenGl
}

# include "glfw3.h" // Load fenetre
# include "stb_image.h" // Load image
# include "glm.hpp" // Implementation matrices

# include "ContextOpenCL.hpp"
# include "Coords.hpp"
# include "Shader.hpp"
# include "Camera.hpp"
# include "Textures.hpp"
# include <iostream>
# include <vector>

# define WIDTH 1920
# define HEIGHT 1080
# define RENDER_DIST 1000.0f
# define VERTEX_SKY "shader/vertexSky.glsl"
# define FRAGMENT_SKY "shader/fragmentSky.glsl"

# define LENGTH_BLOCK 1

enum xz_vec {XZ_X, XZ_Z};
enum xyz_vec {MY, X, Y, Z};

using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

static BlockPos t_cube_pt[] = {
	// +Z // haut gauche -> bas gauche -> bas droite -> haut droite
	BlockPos((int[4]){0,0				, LENGTH_BLOCK	, LENGTH_BLOCK}),
	BlockPos((int[4]){0,0				, 0				, LENGTH_BLOCK}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, 0				, LENGTH_BLOCK}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, LENGTH_BLOCK	, LENGTH_BLOCK}),
	// -Z
	BlockPos((int[4]){0,0				, LENGTH_BLOCK	, 0}),
	BlockPos((int[4]){0,0				, 0				, 0}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, 0				, 0}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, LENGTH_BLOCK	, 0}),
};

typedef struct	s_direction_consts{
	BlockPos	block_vec;
	ChunkPos	chunk_vec;
	short		axis;
	BlockPos	pts[6];
}				t_direction_consts;

# define NB_PTS_CUBE 36

static t_direction_consts	g_dir_c[] = {
	// NORTH
	{BlockPos((int[4]){0,0,0,1}), ChunkPos((int[2]){0, 1}), Z,
		{
			t_cube_pt[0], t_cube_pt[1], t_cube_pt[2],
			t_cube_pt[0], t_cube_pt[2], t_cube_pt[3],
		}
	},
	// EAST
	{BlockPos((int[4]){0,1,0,0}), ChunkPos((int[2]){1, 0}), X,
		{
			t_cube_pt[3], t_cube_pt[2], t_cube_pt[6],
			t_cube_pt[3], t_cube_pt[6], t_cube_pt[7],
		}
	},
	// SOUTH
	{BlockPos((int[4]){0,0,0,-1}), ChunkPos((int[2]){0, -1}), -Z,
		{
			t_cube_pt[4], t_cube_pt[6], t_cube_pt[5],
			t_cube_pt[4], t_cube_pt[7], t_cube_pt[6],
		}
	},
	// WEST
	{BlockPos((int[4]){0,-1,0,0}), ChunkPos((int[2]){-1, 0}), -X,
		{
			t_cube_pt[4], t_cube_pt[5], t_cube_pt[1],
			t_cube_pt[4], t_cube_pt[1], t_cube_pt[0],
		}
	},
	// UP
	{BlockPos((int[4]){0,0,1,0}), ChunkPos((int[2]){0, 0}), Y,
		{
			t_cube_pt[0], t_cube_pt[7], t_cube_pt[4],
			t_cube_pt[0], t_cube_pt[3], t_cube_pt[7],
		}
	},
	// DOWN
	{BlockPos((int[4]){0,0,-1,0}), ChunkPos((int[2]){0, 0}), -Y,
		{
			t_cube_pt[1], t_cube_pt[5], t_cube_pt[6],
			t_cube_pt[1], t_cube_pt[6], t_cube_pt[2],
		}
	}

};

struct vbo_type {
	float			tab[3];
	float			normal[3];
	float			meta;
};

typedef struct	s_add_pt {
	char		dir;
	BlockPos	pts[6];
}				t_add_pt;

class Engine
{
	private:
		GLFWwindow				*window;
		Camera					camera;
		Shader					shader;
		std::vector<Textures*>	textures;
		Shader					shaderSky;
		unsigned				vboSky;
		unsigned				vaoSky;
		bool					sky;
		glm::vec2				mouseLastPos;
		bool					firstMouse;
	public:
		Engine();
		int			initWindow(void);
		GLFWwindow	*getWindow(void);
		void		genSkybox(void);
		bool		isSkybox(void);
		Shader&		getShaderSky(void);
		void		displaySky(Textures *t);
		Camera&		getCam(void);
		void		setCam(Camera cam);
		Shader&		getShader(void);
		void		genTextures(void);
		Textures	*getTexture(unsigned int t);
		void 		fillTextureVector(size_t start, size_t end, bool load);
		void		genBlocksTextures(void);
		void		addTexture(char *pathOrBuffer, unsigned long width, unsigned long height);
		glm::vec2	getMouseLastPos(void);
		void		setMouseLastPos(glm::vec2 v);
		bool		isFirst(void);
		void		setFirst(bool f);
		~Engine();
};

#endif