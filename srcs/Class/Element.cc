/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Element.cc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 21:35:48 by gperez            #+#    #+#             */
/*   Updated: 2020/12/08 22:13:17 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Element.hpp"

Element::Element()
{
	Mat();
	this->vbo = 0;
	this->vao = 0;
	this->offsetTxt = 0;
}

void			Element::setVbo(unsigned int v)
{
	this->vbo = v;
}

unsigned int	Element::getVbo(void)
{
	return (this->vbo);
}

void			Element::setVao(unsigned int v)
{
	this->vao = v;
}

unsigned int	Element::getVao(void)
{
	return (this->vao);
}

void			Element::setOffsetTxt(char offset)
{
	this->offsetTxt = offset;
}

char			Element::getOffsetTxt(void)
{
	return (this->offsetTxt);
}

Element::~Element()
{
	if (this->vbo)
		glDeleteBuffers(1, &(this->vbo));
}