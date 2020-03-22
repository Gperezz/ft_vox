/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/03/22 19:50:36 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"
#include <iostream>

void	key(Engine *env)
{
	if (glfwGetKey(env->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env->getWindow(), true);
}

int		createObj(char *vertexPath, char *fragPath, unsigned int *shaderProg)
{
	char			*info;

	info = NULL;
	if (!(info = (char*)ft_memalloc(1024)))
		return (1);

	if (vertexPath && fragPath)
	{
		if (shader(info, shaderProg, vertexPath, fragPath))
		{
			ft_memdel((void**)&info);
			return (1);
		}
	}
	ft_memdel((void**)&info);
	return (0);
}

void	exec(Engine *env, unsigned int *shaderProg)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	(void)shaderProg;
	key(env);
	glfwSwapBuffers(env->getWindow());
	glfwPollEvents();
}

int		main(void)
{
	Engine env;
	unsigned int	shaderProg;
	// Block block;
	
	// ft_printf(RED"%ld\n" NA, sizeof(block.getInfo()));
	
	initWindow(env.getWindow());

	createObj((char*)VERTEX, (char*)FRAGMENT, &shaderProg);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(!glfwWindowShouldClose(env.getWindow()))
		exec(&env, &shaderProg);
	glDeleteProgram(shaderProg);
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();

	ft_printf(MAGENTA"Ceci est Ft_vox:\n" NA);
	return (0);
}
