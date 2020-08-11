/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:42:19 by gperez            #+#    #+#             */
/*   Updated: 2020/08/11 23:22:23 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_HPP
# define TEXTURES_HPP

enum e_txt {DIRT_T, GRASS_TOP_T, GRASS_SIDE_T, LEAVES_OAK_T, LOG_OAK_T,
	STONE_T, END_BLOCK_T, SKY_T, TEST_T, END_T};

typedef struct s_txt_path
{
	unsigned int	index_txt;
	char			path_txt[34];
}				t_txt_path;

static t_txt_path g_txt_path[] = {
	{DIRT_T, "textures/blocks/dirt.png"},
	{GRASS_TOP_T, "textures/blocks/grass_top.png"},
	{GRASS_SIDE_T, "textures/blocks/grass_side.png"},
	{LEAVES_OAK_T, "textures/blocks/leaves_oak.png"},
	{LOG_OAK_T, "textures/blocks/log_oak.png"},
	{STONE_T, "textures/blocks/stone.png"},
	{END_BLOCK_T, ""},
	{SKY_T, "textures_test/water.bmp"},
	{TEST_T, "textures/blocks/dirt.png"},
	{END_T, ""}
};

class Textures
{
	public:
		Textures();
		Textures(char *txtPath);
		Textures(char *buffer, unsigned long len);
		int				getWidth(void) const;
		int				getHeight(void) const;
		void			setTxt(unsigned int t);
		unsigned int	getTxt(void);
		unsigned char	*getTxtData(void);
		~Textures();
	private:
		int				width;
		int				height;
		int				nrChannels;
		unsigned int	txt;
		unsigned char	*txtData;
};

#endif