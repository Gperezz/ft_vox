/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:14:03 by gperez            #+#    #+#             */
/*   Updated: 2021/11/15 11:33:27 by maiwenn          ###   ########.fr       */
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
	if (block.getInfo().id == AIR || block.getInfo().id == LEAVES || block.getInfo().id == WATER)
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