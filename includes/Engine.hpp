/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:35:15 by gperez            #+#    #+#             */
/*   Updated: 2020/05/05 11:58:58 by gperez           ###   ########.fr       */
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

# include "Shader.hpp"
# include "Camera.hpp"
# include "Textures.hpp"
# include <iostream>
# include <vector>

# define WIDTH 800
# define HEIGHT 600
# define RENDER_DIST 100.0f

class Engine
{
	private:
		GLFWwindow				*window;
		Camera					camera;
		Shader					shader;
		std::vector<Textures*>	textures;
		void					addTexture(char *path);
	public:
		Engine();
		int			initWindow(void);
		GLFWwindow	*getWindow(void);
		Camera&		getCam(void);
		void		setCam(Camera cam);
		Shader&		getShader(void);
		void		genTextures(void);
		Textures	*getTexture(int t);
		~Engine();
};

#endif