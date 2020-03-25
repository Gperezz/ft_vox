/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 17:47:10 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 18:45:30 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.hpp"

Shader::Shader(void)
{
}

int				Shader::createShader(char *info, const char *vertex_path,
	const char *frag_path)
{
	t_shader	shader;
	bool		success;

	if (readShader(&shader, vertex_path, 'v')
		|| readShader(&shader, frag_path, 'f'))
		return (1);
	shader.i_v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader.i_v, 1, (const char *const *)(&(shader.vertex)),
		NULL);
	glCompileShader(shader.i_v);
	glGetShaderiv(shader.i_v, GL_COMPILE_STATUS, &success);
	if (!success)
		return (shaderError(shader.i_v, info, (char*)VERTEX_FAILED));
	shader.i_f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader.i_f, 1, (const char *const *)(&(shader.fragment)),
		NULL);
	glCompileShader(shader.i_f);
	glGetShaderiv(shader.i_f, GL_COMPILE_STATUS, &success);
	if (!success)
		return (shaderError(shader.i_f, info, (char*)FRAGMENT_FAILED));
	Shader::shaderProgram = glCreateProgram();
	glAttachShader(Shader::shaderProgram, shader.i_v);
	glAttachShader(Shader::shaderProgram, shader.i_f);
	glLinkProgram(Shader::shaderProgram);
	glGetProgramiv(Shader::shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
		return (shaderError(Shader::shaderProgram, info, (char*)LINK_SHADER_FAILED));
	glDeleteShader(shader.i_v);
	glDeleteShader(shader.i_f);
	freeTShader(&shader);
	return (0);
}

int				Shader::readShader(t_shader *shader, const char file[], char glsl)
{
	t_lst_str	*lst_str;

	if (!(lst_str = ft_parse_file((char*)file)))
		return (1);
	if (stock_file(lst_str, (glsl == 'v'
		? &(shader->vertex) : &(shader->fragment)), 1))
	{
		free_lst_str(lst_str);
		return (1);
	}
	free_lst_str(lst_str);
	return (0);
}

int				Shader::shaderError(int i_s, char *info, char *error_msg)
{
	glGetShaderInfoLog(i_s, 1024, NULL, info);
	ft_putendl(error_msg);
	ft_putendl(info);
	return (1);
}

void			Shader::freeTShader(t_shader *shader)
{
	ft_memdel((void**)&(shader->vertex));
	ft_memdel((void**)&(shader->fragment));
}

unsigned int	Shader::getShader(void)
{
	return (Shader::shaderProgram);
}

int				Shader::loadShader(char *vertexPath, char *fragPath)
{
	char			*info;

	info = NULL;
	if (!(info = (char*)ft_memalloc(1024)))
		return (1);

	if (vertexPath && fragPath)
	{
		if (shader(info, vertexPath, fragPath))
		{
			ft_memdel((void**)&info);
			return (1);
		}
	}
	ft_memdel((void**)&info);
	return (0);
}

void			Shader::freeProgram(void)
{
	glDeleteProgram(Shader::shaderProgram);
}

Shader::~Shader()
{
}