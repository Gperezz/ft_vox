/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:42:19 by gperez            #+#    #+#             */
/*   Updated: 2021/10/14 14:02:36 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_HPP
# define TEXTURES_HPP

# include "Block.hpp"
# include <iostream>
# include "str_color.hpp"

enum e_txt {DIRT_T, GRASS_TOP_T, GRASS_SIDE_T, LEAVES_OAK_T, LOG_OAK_T,
	STONE_T, END_BLOCK_T, SKY_FRONT_T, SKY_LEFT_T, SKY_BACK_T, SKY_RIGHT_T, 
	SKY_TOP_T, SKY_BOTTOM_T, SKY_T, TEST_T, TXT_HUD, END_T};

typedef struct s_txt_path
{
	e_txt			index_txt;
	e_BlockType		type;
	char			path_txt[34];
}				t_txt_path;

static t_txt_path g_txt_path[] = {
	{DIRT_T, DIRT, "textures/blocks/dirt.png"},
	{GRASS_TOP_T, GRASS, "textures/blocks/grass_top.png"},
	{GRASS_SIDE_T, GRASS, "textures/blocks/grass_side.png"},
	{LEAVES_OAK_T, LEAVES, "textures/blocks/leaves_oak.png"},
	{LOG_OAK_T, LOG, "textures/blocks/log_oak.png"},
	{STONE_T, STONE, "textures/blocks/stone.png"},
	{END_BLOCK_T, (e_BlockType)0, ""},
	{SKY_FRONT_T, (e_BlockType)SKY_DIR_FRONT, "textures/skybox/front.png"},
	{SKY_RIGHT_T, (e_BlockType)SKY_DIR_RIGHT, "textures/skybox/right.png"},
	{SKY_LEFT_T, (e_BlockType)SKY_DIR_LEFT, "textures/skybox/left.png"},
	{SKY_BACK_T, (e_BlockType)SKY_DIR_BACK, "textures/skybox/back.png"},
	{SKY_TOP_T, (e_BlockType)SKY_DIR_UP, "textures/skybox/top.png"},
	{SKY_BOTTOM_T, (e_BlockType)SKY_DIR_DOWN, "textures/skybox/bottom.png"},
	{SKY_T, (e_BlockType)0, ""},
	{TEST_T, (e_BlockType)0, "textures/blocks/dirt.png"},
	{TXT_HUD, (e_BlockType)0, "textures/nbTxt.png"},
	{END_T, (e_BlockType)0, ""}
};

class Textures
{
	public:
		Textures();
		Textures(char *txtPath);
		Textures(char *buf, unsigned long w, unsigned long h);
		// Textures(std::string buf, unsigned long w, unsigned long h);
		void			loadTexture(char *txtPath);
		int				getWidth(void) const;
		int				getHeight(void) const;
		int				getNrChannels(void) const;
		void			setTxt(unsigned int t);
		int				getTxt(void);
		unsigned char	*getTxtData(void);
		static e_txt	getIndexTxt(e_BlockType type);
		~Textures();
	private:
		bool			load;
		int				width;
		int				height;
		int				nrChannels;
		int				txt;
		unsigned char	*txtData;
		bool			isBuffer;
		// std::string		buffer;
};

#endif