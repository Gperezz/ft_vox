/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:42:19 by gperez            #+#    #+#             */
/*   Updated: 2020/10/10 19:02:28 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_HPP
# define TEXTURES_HPP
# include "Block.hpp"

enum e_txt {DIRT_T, GRASS_TOP_T, GRASS_SIDE_T, LEAVES_OAK_T, LOG_OAK_T,
	STONE_T, END_BLOCK_T, SKY_T, TEST_T, END_T};

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
	{SKY_T, (e_BlockType)0, "textures/blocks/dirt.png"},
	{TEST_T, (e_BlockType)0, "textures/blocks/dirt.png"},
	{END_T, (e_BlockType)0, ""}
};

class Textures
{
	public:
		Textures();
		Textures(char *txtPath);
		Textures(char *buffer, unsigned long width, unsigned long height);
		int				getWidth(void) const;
		int				getHeight(void) const;
		void			setTxt(unsigned int t);
		unsigned int	getTxt(void);
		unsigned char	*getTxtData(void);
		static e_txt	getIndexTxt(e_BlockType type);
		~Textures();
	private:
		int				width;
		int				height;
		int				nrChannels;
		unsigned int	txt;
		unsigned char	*txtData;
};

#endif