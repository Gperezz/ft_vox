/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 14:28:56 by gperez            #+#    #+#             */
/*   Updated: 2020/03/22 19:11:47 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

void	free_shader(t_shader *shader)
{
	ft_memdel((void**)&(shader->vertex));
	ft_memdel((void**)&(shader->fragment));
}

int		read_shader(t_shader *shader, const char file[], char glsl)
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

int		shader_error(int i_s, char *info, char *error_msg)
{
	glGetShaderInfoLog(i_s, 1024, NULL, info);
	ft_putendl(error_msg);
	ft_putendl(info);
	return (1);
}

int		shader(char *info, unsigned int *program,
	const char *vertex_path, const char *frag_path)
{
	t_shader	shader;
	int			success;

	if (read_shader(&shader, vertex_path, 'v')
		|| read_shader(&shader, frag_path, 'f'))
		return (1);
	shader.i_v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader.i_v, 1, (const char *const *)(&(shader.vertex)),
		NULL);
	glCompileShader(shader.i_v);
	glGetShaderiv(shader.i_v, GL_COMPILE_STATUS, &success);
	if (!success)
		return (shader_error(shader.i_v, info, (char*)VERTEX_FAILED));
	shader.i_f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader.i_f, 1, (const char *const *)(&(shader.fragment)),
		NULL);
	glCompileShader(shader.i_f);
	glGetShaderiv(shader.i_f, GL_COMPILE_STATUS, &success);
	if (!success)
		return (shader_error(shader.i_f, info, (char*)FRAGMENT_FAILED));
	*program = glCreateProgram();
	glAttachShader(*program, shader.i_v);
	glAttachShader(*program, shader.i_f);
	glLinkProgram(*program);
	glGetProgramiv(*program, GL_LINK_STATUS, &success);
	if (!success)
		return (shader_error(*program, info, (char*)LINK_SHADER_FAILED));
	glDeleteShader(shader.i_v);
	glDeleteShader(shader.i_f);
	free_shader(&shader);
	return (0);
}
