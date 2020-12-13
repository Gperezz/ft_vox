/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hud.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 20:12:28 by gperez            #+#    #+#             */
/*   Updated: 2020/12/13 19:08:08 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hud.hpp"

Hud::Hud()
{
	this->projection = glm::mat4(1);
	this->isInit = false;
}

void		Hud::setProjection(glm::mat4 proj)
{
	this->projection = proj;
}

glm::mat4	Hud::getProjection(void)
{
	return (this->projection);
}

int			Hud::init(glm::mat4 proj)
{
	if (isInit)
		return (0);
	this->projection = proj;
	if (this->shader.loadShader((char*)VERTEX_HUD, (char*)FRAGMENT_HUD))
		return (1);
	this->isInit = true;
	return (0);
}

void		Hud::fillElements(std::vector<char> unitsValue, unsigned int units)
{
	unsigned int iValue = 0;

	for (unsigned int i = 0; i < units && iValue < unitsValue.size(); i++)
	{
		this->hudElements[i]->setOffsetTxt(unitsValue[iValue++]);
	}
}

void		Hud::syncFpsToElements(int fps)
{
	int					nb = fps;
	unsigned int		units;
	std::vector<char>	unitsValue;

	for (units = 0; nb > 0; units++)
	{
		unitsValue.push_back(nb % 10);
		nb /= 10;
	}
	while (hudElements.size() < units)
		newElement();
	while (hudElements.size() > units)
		deleteElement(hudElements.size() - 1);
	fillElements(unitsValue, units);
}

void		Hud::newElement(void)
{
	Element			*element;
	unsigned int	vbo;
	unsigned int	vao;

	element = new Element;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SQUARE_PTS * 2, &square_pt, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(0);
	element->setVbo(vbo);
	element->setVao(vao);
	element->setScale(glm::vec3(0.02, 0.05, 0.0));
	element->setTranslate(glm::vec3(1 - (element->getScale().x + 0.01)
		- (element->getScale().x + 0.05) * (float)this->hudElements.size(),
		1 - (element->getScale().y + 0.01), 0.0));
	this->hudElements.push_back(element);
	glBindVertexArray(0);
}

void		Hud::display(int fps, unsigned int txt)
{
	syncFpsToElements(fps);
	glDepthMask(false);
	for (unsigned int i = 0; i < this->hudElements.size(); i++)
	{
		glUseProgram(this->shader.getProgram());
		glBindVertexArray(this->hudElements[i]->getVao());
		glUniformMatrix4fv(glGetUniformLocation(this->shader.getProgram(),
			"view"), 1, GL_FALSE, glm::value_ptr(this->hudElements[i]->getMatrix(true)));
		glUniformMatrix4fv(glGetUniformLocation(this->shader.getProgram(),
			"projection"), 1, GL_FALSE, glm::value_ptr(this->projection));
		glUniform1i(glGetUniformLocation(this->shader.getProgram(),
			"number"), this->hudElements[i]->getOffsetTxt());
		glUniform1i(glGetUniformLocation(this->shader.getProgram(),
			"basicTexture"), txt);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, SQUARE_PTS);
		glBindVertexArray(0);
	}
	glDepthMask(true);
}

void		Hud::deleteElement(unsigned int i)
{
	if (i >= this->hudElements.size())
		return ;
	delete this->hudElements[i];
	this->hudElements.pop_back();
}

Hud::~Hud()
{
	for (int i = this->hudElements.size() - 1; i >= 0; i--)
	{
		this->deleteElement(i);
	}
	if (this->shader.getProgram())
		this->shader.freeProgram();
}