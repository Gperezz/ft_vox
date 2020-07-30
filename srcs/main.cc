/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/07/30 22:42:33 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

void	key(Engine &env, Mat &world)
{
	if (glfwGetKey(env.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env.getWindow(), true);
	if (glfwGetKey(env.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		world.translate((glm::vec3){0, 0, 0.1});
		world.printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		world.translate((glm::vec3){0, 0, -0.1});
		world.printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		world.translate((glm::vec3){-0.1, 0, 0});
		world.printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		world.translate((glm::vec3){0.1, 0, 0});
		world.printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		world.translate((glm::vec3){0, -0.1, 0});
		world.printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		world.translate((glm::vec3){0, 0.1, 0});
		world.printMatrix(true);
	}
}

void	exec(World &world, Engine &env)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	key(env, world.getWorldMat());
	world.display(env);
	glfwSwapBuffers(env.getWindow());
	glfwPollEvents();
}

// static void	recLoad(World &w, int x, int y)
// {
// 	w.loadChunk(x, y);
// 	if (x < -LIM_GEN || x > LIM_GEN || y < -LIM_GEN || y > LIM_GEN)
// 		return;
// 	recLoad(w, x, y + 1);
// 	recLoad(w, x, y - 1);
// 	recLoad(w, x + 1, y);
// 	recLoad(w, x - 1, y);
// }

int		main(void)
{
	Engine			env;
	World			world;
	Shader&			shader(env.getShader());
	
	// ft_printf(RED"%ld\n" NA, sizeof(block.getInfo()));
	
	env.initWindow();
	shader.loadShader((char*)VERTEX, (char*)FRAGMENT);
	ft_printf(RED"prog %u\n" NA, shader.getProgram());
	env.genTextures();
	env.getCam().setProjMatrix(glm::perspective(glm::radians(45.0f),
		(float)WIDTH / (float)HEIGHT, 0.1f, (float)RENDER_DIST));
	env.getCam().setTranslate((glm::vec3){7.5, 0, -5});
	env.getCam().rotate((glm::vec3){20, 180, 0});
	
	ft_printf(MAGENTA "Cam Matrix\n" NA);
	env.getCam().printMatrix(true);

	ft_printf(MAGENTA "Projection Matrix\n" NA);
	env.getCam().printProjectionMatrix();
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ft_printf(MAGENTA"Ceci est Ft_vox:\n" NA);

	///////////////////////////////////// Load test ////////////////////

	// recLoad(world, 0, 0);
	world.loadChunk(0, 1);
	world.loadChunk(0, -1);
	// world.loadChunk(0, -1);
	world.loadChunk(1, 0);
	world.loadChunk(-1, 0);

	world.loadChunk(0, 0);

	world.loadChunk(0, 0);
	world.loadChunk(2, 0);
	world.loadChunk(1, 1);
	world.loadChunk(1, -1);

	world.loadChunk(1, 0);

	//////////////////////////////////////////////////////////////////////////


	while(!glfwWindowShouldClose(env.getWindow()))
		exec(world, env);
	shader.freeProgram();
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
