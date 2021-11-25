/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2021/11/23 17:04:55 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

void	key(Engine &env, float deltaFrameTime)
{
	float speed = SPEED;
	if (glfwGetKey(env.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env.getWindow(), true);
	if (glfwGetKey(env.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			speed = SPEED_SPRINT;
	else if (glfwGetKey(env.getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			speed = SPEED_ACCEL;
	if (glfwGetKey(env.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		env.getCam().translate(E_FRONT, speed * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		env.getCam().translate(E_FRONT, -speed * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		env.getCam().translate(E_RIGHT, speed * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		env.getCam().translate(E_RIGHT, -speed * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		env.getCam().translate(E_UP, speed * deltaFrameTime);
		// env.getCam().printMatrix(true);
	}
	if (glfwGetKey(env.getWindow(), GLFW_KEY_X) == GLFW_PRESS)
	{
		env.getCam().translate(E_UP, -speed * deltaFrameTime);
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
	// Chunk*			chunk = NULL;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	iImg++;
	timeForFps += world.getDeltaFrameTime();
	key(env, world.getDeltaFrameTime());
	if (checkMouse(env, GLFW_MOUSE_BUTTON_1)
		|| checkMouse(env, GLFW_MOUSE_BUTTON_2))
		return ;
	
	// chunk = world.get(env.getCam().getCurrentChunkPos());
	// if (chunk)
	// 	env.rayCasting(chunk, world); // FAIT SEGFAULT QUAND LE CHUNK OU L ON SE TROUVE N EST PAS GENERER
	
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

int		main(void)
{
	Engine			env;
	World			world(env);
	Shader&			shader(env.getShader());
	TimeMs			time;
	glm::mat4		mat;

	mat = glm::perspective(glm::radians(80.0f),
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
	env.getCam().setTranslate((glm::vec3){7.5, 180., 7.5});

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	time.setTime();

	int	close = 0;
	std::mutex	windowMutex;

	std::cout << GREEN << "Generation du terrain en cours...\n" << NA;
	world.loopGen(false);
	world.loopLoad(false);
	while (world.isStarted())
		world.loadGraphics();

	std::cout << GREEN << "Done !\n" << NA;
	std::thread t0(&World::loopGen, std::ref(world), true);
	std::thread t1(&World::loopLoad, std::ref(world), true);

	while(!close)
	{
		{unique_lock<mutex>		lk(windowMutex);
			close = glfwWindowShouldClose(env.getWindow());
		}
		exec(world, env, time);
	}

	world.end();
	t0.join();
	t1.join();

	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
