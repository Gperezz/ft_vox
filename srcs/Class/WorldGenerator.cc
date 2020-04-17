/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2020/04/17 17:07:26 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldGenerator.hpp"

WorldGenerator::WorldGenerator(void)
{
	this->seed = Rand<unsigned long>().generate();
}

WorldGenerator::WorldGenerator(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
	
}

void	WorldGenerator::genChunk(Chunk chunk)
{
	for (int my = 0; my < 10; my++)
	{
		for (int x = 0; x < 16; x++)
		{
			for (int y = 0; y < 16; y++)
			{
				for (int z = 0; z < 16; z++)
				{
					chunk.get(my, x, y, z) = (t_block_info){1,0,0,0};
				}
			}
		}
	}
}