/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:14:03 by gperez            #+#    #+#             */
/*   Updated: 2020/11/05 11:44:55 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Block.hpp"

Block::Block()
{

}

t_block_info&		Block::getInfo(void)
{
	return (Block::info);
}

bool			Block::isTransparentBlock(Block block)
{
	if (block.getInfo().id == AIR || block.getInfo().id == LEAVES)
		return (true);
	return (false);
}

void			Block::operator=(t_block_info info)
{
	this->info = info;
}

Block::~Block()
{

}