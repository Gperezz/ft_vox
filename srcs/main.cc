/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2021/11/15 11:29:37 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

void	key(Engine &env, float deltaFrameTime)
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

static int	checkMouse(Engine &env, unsigned int b)
{
	if (glfwGetMouseButton(env.getWindow(), b) == GLFW_PRESS
		&& env.getButton(b) == false)
		return (env.setButton(b, true));
	else if (glfwGetMouseButton(env.getWindow(), b) == GLFW_RELEASE
		&& env.getButton(b) == true)
		return (env.setButton(b, false));
	return (0);
}

void	exec(World &world, Engine &env, TimeMs time)
{
	static int		iImg;
	static float	timeForFps;
	static int		imgNb;
	Textures		*t;
	int				idx;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	iImg++;
	timeForFps += world.getDeltaFrameTime();
	key(env, world.getDeltaFrameTime());
	if (checkMouse(env, GLFW_MOUSE_BUTTON_1)
		|| checkMouse(env, GLFW_MOUSE_BUTTON_2))
		return ;
	// env.rayCasting(world.getMapMemory().at(env.getCam().getCurrentChunkPos()),
		// world.getMapMemory());
	world.display(env, time.getTimeSeconds());
	idx = env.getNbTextures() - 1;
	t = env.getTexture(idx);
	env.getHud().display(imgNb, t ? t->getTxt() : 0);
	if (timeForFps > 1.0f - PREC)
	{
		imgNb = iImg;
		timeForFps = 0.0;
		iImg = 0;
	}
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
	World			world(env);
	Shader&			shader(env.getShader());
	TimeMs			time;
	glm::mat4		mat;

	mat = glm::perspective(glm::radians(45.0f),
		(float)WIDTH / (float)HEIGHT, 0.1f, (float)RENDER_DIST);
	if (env.initWindow() == -1)
		return (1);
	if (shader.loadShader((char*)VERTEX, (char*)FRAGMENT))
		return (1);
	if (env.genTextures())
		return (1);
	if (env.genSkybox())
		return (1);
	if (env.getHud().init(glm::mat4(1)))
		return (1);
	env.getCam().setProjMatrix(mat);
	env.getCam().setTranslate((glm::vec3){7.5, 35, 7.5});

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// int lim = 1;

	///////////////////////////////////// Load test ////////////////////

	// for (int y = 0; y < lim; y++)
	// {
	// 	for (int x = 0; x < lim; x++)
	// 	{
	// 	}
	// }
	//////////////////////////////////////////////////////////////////////////

	recLoad(world, 0, 0, 0);

	time.setTime();
	while(!glfwWindowShouldClose(env.getWindow()))
		exec(world, env, time);
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
