/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.cc                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:54:04 by gperez            #+#    #+#             */
/*   Updated: 2021/12/01 19:19:49 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Textures.hpp"

# define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

t_txt_path g_txt_path[] = {
	{DIRT_T, DIRT, "textures/blocks/dirt.png"},
	{GRASS_TOP_T, GRASS, "textures/blocks/grass_top.png"},
	{GRASS_SIDE_T, GRASS, "textures/blocks/grass_side.png"},
	{LEAVES_OAK_T, LEAVES, "textures/blocks/leaves_oak.png"},
	{LOG_OAK_T, LOG, "textures/blocks/log_oak.png"},
	{STONE_T, STONE, "textures/blocks/stone.png"},
	{SAND_T, SAND, "textures/blocks/sand.png"},
	{SNOW_T, SNOW, "textures/blocks/snow.png"},
	{WATER_T, WATER, "textures/blocks/water_still.png"},
	{END_BLOCK_T, (e_BlockType)0, ""},
	{SKY_FRONT_T, (e_BlockType)SKY_DIR_FRONT, "textures/skybox/front.png"},
	{SKY_LEFT_T, (e_BlockType)SKY_DIR_LEFT, "textures/skybox/left.png"},
	{SKY_BACK_T, (e_BlockType)SKY_DIR_BACK, "textures/skybox/back.png"},
	{SKY_RIGHT_T, (e_BlockType)SKY_DIR_RIGHT, "textures/skybox/right.png"},
	{SKY_TOP_T, (e_BlockType)SKY_DIR_UP, "textures/skybox/top.png"},
	{SKY_BOTTOM_T, (e_BlockType)SKY_DIR_DOWN, "textures/skybox/bottom.png"},
	{SKY_T, (e_BlockType)0, ""},
	{TEST_T, (e_BlockType)0, "textures/blocks/dirt.png"},
	{TXT_HUD, (e_BlockType)0, "textures/nbTxt.png"},
	{END_T, (e_BlockType)0, ""}
};

Textures::Textures()
{
	this->load = false;
	this->width = 0;
	this->height = 0;
	this->nrChannels = 0;
	this->txt = -1;
	this->txtData = NULL;
	this->isBuffer = false;
}

Textures::Textures(char *txtPath)
{
	this->load = false;
	this->txt = -1;
	this->txtData = NULL;
	this->isBuffer = false;
	this->loadTexture(txtPath);
}

Textures::Textures(char *buf, unsigned long w, unsigned long h)
{
	this->isBuffer = true;
	this->txtData = (unsigned char*)buf;
	this->width = w;
	this->height = h;
	this->load = true;
}

void	Textures::loadTexture(char *txtPath)
{
	if (this->load)
		return;
	stbi_set_flip_vertically_on_load(true);
	this->txtData = stbi_load(txtPath, &this->width, &this->height, &this->nrChannels, 0);
	printf(CYAN "Height:%d Width%d\n" NA, this->height, this->width);
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
	if (this->txtData && !isBuffer)
		stbi_image_free(this->txtData);
}