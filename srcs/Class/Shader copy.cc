/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/25 17:47:10 by gperez            #+#    #+#             */
/*   Updated: 2021/10/08 18:42:18 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.hpp"

Shader::Shader(void)
{
	this->shaderProgram = 0;
}

static void			freeTShader(t_shader &shader)
{
	ft_memdel((void**)&(shader.vertex));
	ft_memdel((void**)&(shader.fragment));
}

static int				readShader(t_shader &shader, const char file[], char glsl)
{
	t_lst_str	*lst_str;

	if (!(lst_str = ft_parse_file((char*)file)))
		return (1);
	if (stock_file(lst_str, (glsl == 'v'
		? &(shader.vertex) : &(shader.fragment)), 1))
	{
		free_lst_str(lst_str);
		return (1);
	}
	free_lst_str(lst_str);
	return (0);
}

static int			shaderError(t_shader &sh, int i_s, char *info, char *error_msg)
{
	glGetShaderInfoLog(i_s, 1024, NULL, info);
	freeTShader(sh);
	ft_putendl(error_msg);
	ft_putendl(info);
	return (1);
}

int				Shader::createShader(char *info, const char *vertex_path,
	const char *frag_path)
{
	t_shader		shader;
	int				success;

	bzero(&shader, sizeof(t_shader));
	if (readShader(shader, vertex_path, 'v')
		|| readShader(shader, frag_path, 'f'))
	{
		freeTShader(shader);
		return (1);
	}
	shader.i_v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader.i_v, 1, (const char *const *)(&(shader.vertex)),
		NULL);
	glCompileShader(shader.i_v);
	glGetShaderiv(shader.i_v, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		shaderError(shader, shader.i_v, info, (char*)VERTEX_FAILED);
		glDeleteShader(shader.i_v);
		return (1);
	}
	shader.i_f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader.i_f, 1, (const char *const *)(&(shader.fragment)),
		NULL);
	glCompileShader(shader.i_f);
	glGetShaderiv(shader.i_f, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		shaderError(shader, shader.i_f, info, (char*)FRAGMENT_FAILED);
		glDeleteShader(shader.i_v);
		glDeleteShader(shader.i_f);
		return (1);
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shader.i_v);
	glAttachShader(shaderProgram, shader.i_f);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		shaderError(shader, shaderProgram, info, (char*)LINK_SHADER_FAILED);
		glDeleteShader(shader.i_v);
		glDeleteShader(shader.i_f);
		return (1);
	}
	glDeleteShader(shader.i_v);
	glDeleteShader(shader.i_f);
	freeTShader(shader);
	return (0);
}


unsigned int	Shader::getProgram(void)
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
		if (createShader(info, vertexPath, fragPath))
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
	if (this->shaderProgram == 0)
		return;
	glDeleteProgram(Shader::shaderProgram);
	this->shaderProgram = 0;
}

Shader::~Shader()
{
}