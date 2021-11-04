/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:14:08 by gperez            #+#    #+#             */
/*   Updated: 2021/11/04 15:12:23 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BLOCK_HPP_
# define _BLOCK_HPP_

extern "C"
{
	# include "glad.h" // Implementation OpenGl
}

# include "glfw3.h" // Load fenetre
# include "stb_image.h" // Load image
# include "glm.hpp" // Implementation matrices

enum	e_BlockType : char {
	AIR,
	STONE,
	DIRT,
	GRASS,
	LOG,
	LEAVES,
	WATER,
	// COAL_ORE,
	// IRON_ORE,
	// GOLD_ORE,
	// DIAMOND_ORE,
	// COBBLESTONE
};

enum	e_SkyDir : char {
	SKY_DIR_FRONT,
	SKY_DIR_LEFT,
	SKY_DIR_BACK,
	SKY_DIR_RIGHT,
	SKY_DIR_UP,
	SKY_DIR_DOWN,
};

typedef struct		block_info{
	unsigned char	id;
	unsigned		variation : 4;
	unsigned		direction : 2;
	unsigned		rev : 1;
}					t_block_info;

class Block {
	private:
		t_block_info	info;
	public:
						Block();
						~Block();
		t_block_info&	getInfo(void);
		static bool		isTransparentBlock(Block block);
		void			operator=(t_block_info);
};

#endif