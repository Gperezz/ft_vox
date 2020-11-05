/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:14:08 by gperez            #+#    #+#             */
/*   Updated: 2020/11/05 11:41:53 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BLOCK_HPP_
# define _BLOCK_HPP_

extern "C"
{
	# include "ft_printf.h"
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