/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/11/09 22:48:55 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

void	key(Engine &env, const float deltaFrameTime)
{
	if (glfwGetKey(env.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env.getWindow(), true);
	if (glfwGetKey(env.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		env.getCam().translate(E_FRONT, SPEED * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		env.getCam().translate(E_FRONT, -SPEED * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		env.getCam().translate(E_RIGHT, SPEED * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		env.getCam().translate(E_RIGHT, -SPEED * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		env.getCam().translate(E_UP, SPEED * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		env.getCam().translate(E_UP, -SPEED * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
}

void	exec(World &world, Engine &env, TimeMs time)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	key(env, world.getDeltaFrameTime());
	world.display(env, time.getTimeSeconds());
	glfwSwapBuffers(env.getWindow());
	glfwPollEvents();
}

static void	recLoad(World &w, int x, int y, int rec)
{
	w.loadChunk(x, y);
	if (rec == 1)
		return;
	recLoad(w, x, y + 1, 1);
	recLoad(w, x, y - 1, 1);
	recLoad(w, x + 1, y, 1);
	recLoad(w, x - 1, y, 1);
}

int		main(void)
{
	Engine			env;
	World			world;
	Shader&			shader(env.getShader());
	TimeMs			time;

	// ft_printf(RED"%ld\n" NA, sizeof(block.getInfo()));

	if (env.initWindow() == -1)
		return (1);
	if (shader.loadShader((char*)VERTEX, (char*)FRAGMENT))
		return (1);
	if (env.genTextures() || env.genSkybox())
		return (1);
	env.getCam().setProjMatrix(glm::perspective(glm::radians(45.0f),
		(float)WIDTH / (float)HEIGHT, 0.1f, (float)RENDER_DIST));
	env.getCam().setTranslate((glm::vec3){7.5, 1, 2});

	ft_printf(MAGENTA "Cam Matrix\n" NA);
	env.getCam().printMatrix(true);

	ft_printf(MAGENTA "Projection Matrix\n" NA);
	env.getCam().printProjectionMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	ft_printf(MAGENTA"Ceci est Ft_vox:\n" NA);

	int lim = 1;

	///////////////////////////////////// Load test ////////////////////

	for (int y = 0; y < lim; y++)
	{
		for (int x = 0; x < lim; x++)
		{
			recLoad(world, x, y, 0);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	time.setTime();
	while(!glfwWindowShouldClose(env.getWindow()))
		exec(world, env, time);
	shader.freeProgram();
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
