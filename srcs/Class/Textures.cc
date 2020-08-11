/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:54:04 by gperez            #+#    #+#             */
/*   Updated: 2020/08/11 22:39:37 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Textures.hpp"

# define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

Textures::Textures()
{
	this->width = 0;
	this->height = 0;
	this->nrChannels = 0;
	this->txt = -1;
	this->txtData = NULL;
}

Textures::Textures(char *txtPath)
{
	stbi_set_flip_vertically_on_load(true);
	this->txtData = stbi_load(txtPath, &width, &height, &nrChannels, 0);
}

Textures::Textures(char *buffer, unsigned long len)
{
	this->txtData = stbi_load_from_memory((const stbi_uc *)buffer, len, &width, &height, &nrChannels, 0);
}

int				Textures::getWidth(void) const
{
	return (this->width);
}

int				Textures::getHeight(void) const
{
	return (this->height);
}

void			Textures::setTxt(unsigned int t)
{
	this->txt = t;
}

unsigned int	Textures::getTxt(void)
{
	return (this->txt);
}

unsigned char	*Textures::getTxtData(void)
{
	return (this->txtData);
}

Textures::~Textures()
{
	stbi_image_free(this->txtData);
}