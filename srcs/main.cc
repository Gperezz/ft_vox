/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cc                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:14 by gperez            #+#    #+#             */
/*   Updated: 2020/04/13 15:46:56 by gperez           ###   ########.fr       */
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

void	generateVao(unsigned int *vao)
{
	unsigned int vbo;
	unsigned int ebo;

	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TEST), TEST, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EBO),
		EBO, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(int), (void*)0);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(float) * 3 + sizeof(int), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
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
