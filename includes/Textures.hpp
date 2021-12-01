/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:42:19 by gperez            #+#    #+#             */
/*   Updated: 2021/11/27 23:02:11 by maiwenn          ###   ########.fr       */
/*   Updated: 2021/11/26 12:19:13 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_HPP
# define TEXTURES_HPP

# include "Block.hpp"
# include <iostream>
# include "str_color.hpp"

enum e_txt {DIRT_T, GRASS_TOP_T, GRASS_SIDE_T, LEAVES_OAK_T, LOG_OAK_T,
	STONE_T, SAND_T, END_BLOCK_T, SKY_FRONT_T, SKY_LEFT_T, SKY_BACK_T, SKY_RIGHT_T, 
	SKY_TOP_T, SKY_BOTTOM_T, SKY_T, TEST_T, TXT_HUD, END_T};

typedef struct s_txt_path
{
	e_txt			index_txt;
	e_BlockType		type;
	char			path_txt[34];
}				t_txt_path;

typedef struct s_id
{
	e_txt		txt;
	e_BlockType	type;
}				t_id;

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