/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:35:15 by gperez            #+#    #+#             */
/*   Updated: 2021/11/30 17:12:46 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ENGINE_HPP_
# define _ENGINE_HPP_

extern "C"
{
	# include "glad.h" // Implementation OpenGl
}

# include "glfw3.h" // Load fenetre
# include "stb_image.h" // Load image
# include "glm.hpp" // Implementation matrices

# include "World.hpp"
# include "Chunk.hpp"
# include "Hud.hpp"

# include <iostream>
# include <vector>

# define WIDTH  2560
# define HEIGHT 1600
# define RENDER_DIST 1000.0f
# define VERTEX_SKY "shader/vertexSky.glsl"
# define FRAGMENT_SKY "shader/fragmentSky.glsl"
# include <queue>

enum	e_vsync {VSYNC_OFF, VSYNC_ON};
enum	e_stateKey {KEY_PRESS, KEY_DONE, KEY_RELEASE};

class Engine
{
	private:
		GLFWwindow				*window;
		// Keys //
		bool					isCursor;
		std::queue<char>		queue;
		char					keys[GLFW_KEY_END];
		void					inputKey(unsigned int key);
		bool					buttons[GLFW_MOUSE_BUTTON_LAST + 1];
	
		Camera					camera;
		Shader					shader;
		std::vector<Textures*>	textures;
		Shader					shaderSky;
		unsigned				vboSky;
		unsigned				vaoSky;
		bool					sky;
		glm::vec2				mouseLastPos;
		bool					firstMouse;
		Hud						hud;
		bool					lockRay;
		Block					*getBlockFromPos(Chunk **chunk, glm::vec3 pos, glm::vec4 &bP, World &world);

	public:
		Engine();
		void			getKeys(float deltaFrameTime);
		void			checkKeys(World &wolrd);
		void			rayCasting(World &world);
		int				initWindow(void);
		GLFWwindow		*getWindow(void);
		int				setButton(unsigned int b, bool value);
		bool			getButton(unsigned int b);
		Hud&			getHud(void);
		int				genSkybox(void);
		bool			isSkybox(void);
		Shader&			getShaderSky(void);
		void			displaySky(Textures *t);
		Camera&			getCam(void);
		void			setCam(Camera cam);
		Shader&			getShader(void);
		int				genTextures(void);
		Textures		*getTexture(unsigned int t);
		unsigned int	getNbTextures(void);
		void 			fillTextureVector(size_t start, size_t end, bool load);
		int				genBlocksTextures(glm::vec2 len, e_txt start, e_txt end, size_t offsetInTexture);
		void			addTexture(char *path);
		void			addTexture(char *buffer, unsigned long width, unsigned long height);
		// void			addTexture(std::string buffer, unsigned long width, unsigned long height);
		glm::vec2		getMouseLastPos(void);
		void			setMouseLastPos(glm::vec2 v);
		bool			isFirst(void);
		void			setFirst(bool f);
		~Engine();
};

#endif