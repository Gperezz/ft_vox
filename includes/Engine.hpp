/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:35:15 by gperez            #+#    #+#             */
/*   Updated: 2020/04/14 21:42:45 by gperez           ###   ########.fr       */
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

# include "World.hpp"
# include "Shader.hpp"
# include "Camera.hpp"
# include <iostream>

# define WIDTH 800
# define HEIGHT 600
# define RENDER_DIST 100.0f

struct vbo_test {
	float tab[3];
	unsigned meta;
};

static struct vbo_test TEST[] = {
	{{-1.0, -1.0, 0.0}, 1324124124},
	{{1.0, -1.0, 0.0}, 123412},
	{{0.0, 1.0, 0.0}, 23434654},
	{{2.0, 0.0, 0.0}, 23434654},
	{{-1.0, -1.0, 0.0}, 124124124},
	{{1.0, -1.0, 0.0}, 12341243},
	{{0.0, 1.0, 0.0}, 23434654}
};

// static unsigned int EBO[] = {
// 	0, 1, 2, 9,
// 	3, 4, 5, 9,
// 	6, 7, 8, 9
// };

static unsigned int EBO[] = {
	0, 1, 2
};

class Engine
{
	private:
		GLFWwindow	*window;
		Camera		camera;
		// World		world;
	public:
		Engine();
		int			initWindow(void);
		GLFWwindow	*getWindow(void);
		Camera		getCam(void);
		void		setCam(Camera cam);
		~Engine();
};

#endif