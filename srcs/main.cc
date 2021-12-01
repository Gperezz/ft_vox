/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2021/12/01 15:38:09 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

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
	env.getKeys(world.getDeltaFrameTime());
	env.checkKeys(world);
	if (checkMouse(env, GLFW_MOUSE_BUTTON_1)
		|| checkMouse(env, GLFW_MOUSE_BUTTON_2))
		return ;

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



int		main(int argc, char *argv[])
{
	Engine			env;
	Shader&			shader(env.getShader());
	TimeMs			time;
	glm::mat4		mat;
	unsigned long	seed = SEED;

	if (argc == 2)
	{
		std::string str = std::string(argv[1]);
		if (str.size() < 20)
		{
			for (std::string::iterator it = str.begin(); it != str.end();)
			{
				if (!std::isalnum((int)(*it)))
					it = str.erase(it);
				else
					it++;
			}
			try{
				if (str.size())
					seed = std::stoul(str, 0, 36);
			}
			catch(Error const &e){
				std::cout << e.what() << "\n";
				return (1);
			}
			if (seed < MIN_SEED)
			{
				seed = SEED;
				std::cout << "Seed too short\n" << NA;
			}
		}
		else
			std::cout << "Argument is too long\n" << NA;
	}

	World			world(env, &seed);

	if (env.initWindow() == -1)
		return (1);
	mat = glm::perspective(glm::radians(80.0f),
		(float)env.getWidth() / (float)env.getHeight(), 0.1f, (float)RENDER_DIST);
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
		if (world.isEnd())
			close = 1;
	}

	world.end();
	t0.join();
	t1.join();
	env.deleteText();
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
