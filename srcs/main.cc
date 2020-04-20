/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/04/20 03:21:39 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

void	key(Engine &env)
{
	if (glfwGetKey(env.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env.getWindow(), true);
}

void	exec(World &world, Engine &env)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	key(env);
	world.display(env);
	glfwSwapBuffers(env.getWindow());
	glfwPollEvents();
}

int		main(void)
{
	Engine			env;
	World			world;
	Shader&			shader(env.getShader());
	
	// ft_printf(RED"%ld\n" NA, sizeof(block.getInfo()));
	
	env.initWindow();
	shader.loadShader((char*)VERTEX, (char*)FRAGMENT);
	ft_printf(RED"prog %u\n" NA, shader.getProgram());
	env.getCam().setProjMatrix(glm::perspective(glm::radians(45.0f),
		(float)WIDTH / (float)HEIGHT, 0.1f, (float)RENDER_DIST));
	env.getCam().translate((glm::vec3){0.0, 30.0, 0.0});
	env.getCam().rotate((glm::vec3){45, 0, 0});
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ft_printf(MAGENTA"Ceci est Ft_vox:\n" NA);
	world.loadChunk(0, 1);
	world.loadChunk(0, -1);
	world.loadChunk(0, -1);
	world.loadChunk(1, 0);
	world.loadChunk(-1, 0);
	world.loadChunk(0, 0);
	while(!glfwWindowShouldClose(env.getWindow()))
		exec(world, env);
	shader.freeProgram();
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
