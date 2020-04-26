/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:52:39 by gperez            #+#    #+#             */
/*   Updated: 2020/04/26 11:04:54 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

using namespace std;

Engine::Engine()
{
	
}

static void		framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

int			Engine::initWindow(void)
{
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return (-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	Engine::window = glfwCreateWindow(WIDTH, HEIGHT, "ft_vox", NULL, NULL);
	if (Engine::window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return (-1);
	}
	glfwMakeContextCurrent(Engine::window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return (-1);
	}
	glfwSetFramebufferSizeCallback(Engine::window, framebuffer_size_callback);
	glfwSwapInterval(1);
	return (0);
}

GLFWwindow	*Engine::getWindow(void)
{
	return (Engine::window);
}

Camera&		Engine::getCam(void)
{
	return (Engine::camera);
}
void		Engine::setCam(Camera cam)
{
	Engine::camera = cam;
}

Shader&		Engine::getShader(void)
{
	return (Engine::shader);
}

Engine::~Engine()
{
}