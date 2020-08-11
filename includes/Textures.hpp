/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:42:19 by gperez            #+#    #+#             */
/*   Updated: 2020/08/11 19:47:24 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_HPP
# define TEXTURES_HPP

enum e_txt {DIRT_T, GRASS_TOP_T, GRASS_SIDE_T, LEAVES_OAK_T, LOG_OAK_T,
	STONE_T, END_BLOCK_T, SKY_T, END_T};

typedef struct s_txt_path
{
	unsigned int	index_txt;
	char			path_txt[34];
}				t_txt_path;

static t_txt_path g_txt_path[] = {
	{DIRT_T, "blocks/dirt.png"},
	{GRASS_TOP_T, "blocks/grass_top.png"},
	{GRASS_SIDE_T, "blocks/grass_side.png"},
	{LEAVES_OAK_T, "blocks/leaves_oak.png"},
	{LOG_OAK_T, "blocks/log_oak.png"},
	{STONE_T, "blocks/stone.png"},
	{END_BLOCK_T, ""},
	{SKY_T, "textures/water.bmp"},
	{END_T, ""}
};

class Textures
{
	public:
		Textures();
		Textures(char *txtPath);
		Textures(char *buffer, size_t len);
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