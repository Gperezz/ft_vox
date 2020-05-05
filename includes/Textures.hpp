/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Textures.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 17:42:19 by gperez            #+#    #+#             */
/*   Updated: 2020/04/28 21:29:33 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_HPP
# define TEXTURES_HPP

enum e_txt {DEFAULT_TXT, DEFAULT_TXT_PLAYER, WALL_TXT_SIDE, GROUND_TXT, OBS_TXT, NUMB_TXT, END_TXT};

typedef struct s_txt_path
{
	unsigned int	index_txt;
	char			path_txt[22];
}				t_txt_path;

static t_txt_path g_txt_path[] = {
	{DEFAULT_TXT, "textures/world.bmp"},
	{DEFAULT_TXT_PLAYER, "textures/grains.bmp"},
	{WALL_TXT_SIDE, "textures/bricks.bmp"},
	{GROUND_TXT, "textures/water.bmp"},
	{OBS_TXT, "textures/grains2.bmp"},
	{NUMB_TXT, "textures/nbTxt.png"},
	{END_TXT, ""}
};

class Textures
{
	public:
		Textures();
		Textures(char *txtPath);
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