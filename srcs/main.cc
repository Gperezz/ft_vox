/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 19:05:57 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"
#include <iostream>

void	key(Engine *env)
{
	if (glfwGetKey(env->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env->getWindow(), true);
}

void	exec(Engine *env)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	key(env);
	glfwSwapBuffers(env->getWindow());
	glUseProgram(env->getShader().getProgram());
	glfwPollEvents();
}

int		main(void)
{
	Engine env;
	
	// ft_printf(RED"%ld\n" NA, sizeof(block.getInfo()));
	
	env.initWindow();
	env.getShader().loadShader((char*)VERTEX, (char*)FRAGMENT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(!glfwWindowShouldClose(env.getWindow()))
		exec(&env);
	env.getShader().freeProgram();
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();

	ft_printf(MAGENTA"Ceci est Ft_vox:\n" NA);
	return (0);
}
