/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:54:04 by gperez            #+#    #+#             */
/*   Updated: 2020/12/13 18:51:57 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Textures.hpp"

# define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

Textures::Textures()
{
	this->load = false;
	this->width = 0;
	this->height = 0;
	this->nrChannels = 0;
	this->txt = -1;
	this->txtData = NULL;
}

Textures::Textures(char *txtPath)
{
	this->loadTexture(txtPath);
}

Textures::Textures(char *buffer, unsigned long width, unsigned long height)
{
	this->txtData = (unsigned char*)buffer;
	this->width = width;
	this->height = height;
	this->load = true;
}

void	Textures::loadTexture(char *txtPath)
{
	if (this->load)
		return;
	stbi_set_flip_vertically_on_load(true);
	this->txtData = stbi_load(txtPath, &width, &height, &nrChannels, 0);
	this->load = true;
}


int				Textures::getWidth(void) const
{
	return (this->width);
}

int				Textures::getHeight(void) const
{
	return (this->height);
}

int				Textures::getNrChannels(void) const
{
	return (this->nrChannels);
}

void			Textures::setTxt(unsigned int t)
{
	this->txt = t;
}

int				Textures::getTxt(void)
{
	return (this->txt);
}

unsigned char	*Textures::getTxtData(void)
{
	return (this->txtData);
}

e_txt			Textures::getIndexTxt(e_BlockType type)
{
	e_txt	index = (e_txt)0;
	bool	find = false;

	for (int i = 0; find == false && i < END_T; i++)
	{
		if (g_txt_path[i].type == type)
		{
			index = g_txt_path[i].index_txt;
			find = true;
		}
	}
	return (index);
}

Textures::~Textures()
{
	if (this->txtData)
		stbi_image_free(this->txtData);
}