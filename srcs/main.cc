/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/04/14 01:38:17 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"
#include <iostream>

void	key(Engine *env)
{
	if (glfwGetKey(env->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(env->getWindow(), true);
}

void	exec(Engine *env, unsigned vao, Shader shader)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	key(env);
	glBindVertexArray(vao);
	glUseProgram(shader.getProgram());
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
		"view"), 1, GL_FALSE, glm::value_ptr(env->getCam().getMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
		"projection"), 1, GL_FALSE, glm::value_ptr(env->getCam().getProjMatrix()));

	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(env->getWindow());
	glfwPollEvents();
}

int		main(void)
{
	Engine			env;
	Shader			shader;
	unsigned int	vao;
	
	// ft_printf(RED"%ld\n" NA, sizeof(block.getInfo()));
	
	vao = 0;
	env.initWindow();
	shader.loadShader((char*)VERTEX, (char*)FRAGMENT);
	ft_printf(RED"prog %u\n" NA, shader.getProgram());
	env.getCam().setProjMatrix(glm::perspective(glm::radians(45.0f),
		(float)WIDTH / (float)HEIGHT, 0.1f, (float)RENDER_DIST));

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ft_printf(MAGENTA"Ceci est Ft_vox:\n" NA);
	
	ft_printf(RED"HERE %u\n" NA, vao);
	generateVao(&vao);
	ft_printf(RED"HERE %u\n" NA, vao);
	
	ft_printf(RED"prog %u\n" NA, shader.getProgram());
	while(!glfwWindowShouldClose(env.getWindow()))
		exec(&env, vao, shader);
	shader.freeProgram();
	glfwDestroyWindow(env.getWindow());
	glfwTerminate();
	return (0);
}
