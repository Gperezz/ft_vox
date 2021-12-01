/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hud.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 20:12:28 by gperez            #+#    #+#             */
/*   Updated: 2021/11/27 22:38:40 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hud.hpp"

static float	square_pt[] = {
	-1.0, 1.0,
	-1.0, -1.0,
	1.0, 1.0,
	1.0, -1.0
};

Hud::Hud()
{
	this->projection = glm::mat4(1);
	this->isInit = false;
	this->cursorColor = WHITE_CURSOR;
}

void		Hud::setProjection(glm::mat4 proj)
{
	this->projection = proj;
}

glm::mat4	Hud::getProjection(void)
{
	return (this->projection);
}

void		Hud::setCursorColor(char color)
{
	this->cursorColor = color;
}

char		Hud::getCursorColor(void)
{
	return (this->cursorColor);
}

int			Hud::init(glm::mat4 proj)
{
	if (isInit)
		return (0);
	this->projection = proj;
	if (this->shader.loadShader((char*)VERTEX_HUD, (char*)FRAGMENT_HUD))
		return (1);
	this->isInit = true;
	newElement();
	this->hudElements[0]->setOffsetTxt(11);
	this->hudElements[0]->setTranslate((glm::vec3){0.0, 0.0, 0.0});
	this->hudElements[0]->setScale((glm::vec3){0.02, 0.05, 0.0});
	return (0);
}

void		Hud::fillElements(std::vector<char> unitsValue, unsigned int units)
{
	unsigned int iValue = 0;

	for (unsigned int i = 1; i < units + 1 && iValue < unitsValue.size(); i++)
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
	while (hudElements.size() < units + 1)
		newElement();
	while (hudElements.size() > units + 1)
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
		- (element->getScale().x + 0.05) * ((float)this->hudElements.size() - 1),
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
			"cursorColor"), this->cursorColor);
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
	this->hudElements.erase(this->hudElements.begin() + i);
}

Shader&		Hud::getShader(void)
{
	return (this->shader);
}

void		Hud::deleteHud(void)
{
	for (int i = this->hudElements.size() - 1; i >= 0; i--)
		delete this->hudElements[i];
	if (this->shader.getProgram())
		this->shader.freeProgram();
}

Hud::~Hud()
{
}