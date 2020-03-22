/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:14:08 by gperez            #+#    #+#             */
/*   Updated: 2020/03/22 19:15:03 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BLOCK_HPP_
# define _BLOCK_HPP_

typedef struct		block_info{
	unsigned char	id;
	unsigned 		face : 3;
	unsigned		direction : 3;
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