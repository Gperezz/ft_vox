/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:14:08 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 19:03:06 by gperez           ###   ########.fr       */
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

typedef struct		block_info{
	unsigned char	id;
	unsigned		variation : 4;
	unsigned		direction : 2;
	unsigned		rev : 1;
}					t_block_info;

class Block {
	private:
		Block();
		t_block_info info;
		~Block();
	public:
		t_block_info getInfo(void);
};

#endif