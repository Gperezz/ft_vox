/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 17:47:37 by gperez            #+#    #+#             */
/*   Updated: 2020/04/01 16:50:28 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SHADER_HPP_
# define _SHADER_HPP_

extern "C"
{
	# include "ft_printf.h"
	# include "glad.h" // Implementation OpenGl
}

# define VERTEX "shader/vertex.glsl"
# define FRAGMENT "shader/fragment.glsl"

# define VERTEX_FAILED "Shader vertex compilation failed"
# define FRAGMENT_FAILED "Shader fragment compilation failed"
# define LINK_SHADER_FAILED "Shader link failed"


typedef struct				s_shader
{
	char			*path_vertex;
	char			*vertex;
	unsigned int	i_v;
	char			*path_fragment;
	char			*fragment;
	unsigned int	i_f;
}							t_shader;

class Shader
{
	private:
		unsigned int	shaderProgram;
		int				createShader(char *info, const char *vertex_path,
							const char *frag_path);
		int				readShader(t_shader *shader, const char file[], char glsl);
		int				shaderError(int i_s, char *info, char *errorMsg);
		void			freeTShader(t_shader *shader);
	public:
		Shader(void);
		unsigned int	getProgram(void);
		int				loadShader(char *vertexPath, char *fragPath);
		void			freeProgram(void);
		~Shader();
};

#endif