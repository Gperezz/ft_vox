/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vox.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:41 by gperez            #+#    #+#             */
/*   Updated: 2020/03/22 19:50:14 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_VOX_HPP_
# define _FT_VOX_HPP_

extern "C"
{
	# include "ft_printf.h"
	# include "glad.h" // Implementation OpenGl
}

# include "glfw3.h" // Load fenetre
# include "stb_image.h" // Load image
# include "glm.hpp" // Implementation matrices

# define WIDTH 800
# define HEIGHT 600

# define VERTEX_FAILED "Shader vertex compilation failed"
# define FRAGMENT_FAILED "Shader fragment compilation failed"
# define LINK_SHADER_FAILED "Shader link failed"

# define VERTEX "shader/vertex.glsl"
# define FRAGMENT "shader/fragment.glsl"

# include "Engine.hpp"


int initWindow(GLFWwindow *window);

int		shader(char *info, unsigned int *program,
	const char *vertex_path, const char *frag_path);

typedef struct				s_shader
{
	char			*path_vertex;
	char			*vertex;
	unsigned int	i_v;
	char			*path_fragment;
	char			*fragment;
	unsigned int	i_f;
}							t_shader;

#endif