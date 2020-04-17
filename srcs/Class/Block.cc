/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:14:03 by gperez            #+#    #+#             */
/*   Updated: 2020/04/17 17:09:18 by gperez           ###   ########.fr       */
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

Block::~Block()
{

}

void			Block::operator=(t_block_info info)
{
	this->info = info;
}
