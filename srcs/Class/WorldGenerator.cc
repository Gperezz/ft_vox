/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2020/10/10 19:07:37 by gperez           ###   ########.fr       */
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

void	WorldGenerator::genTest(Chunk *chunk)
{
	for (int my = 0; my < 16; my++)
	{
		for (int y = 0; y < 16; y++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int x = 0; x < 16; x++)
				{
					chunk->setBlock(BlockPos((int[4]){my, x, y, z}),
						(t_block_info){STONE,0,0,0});
				}
			}
		}
	}
	chunk->setBlock(BlockPos((int[4]){7, 7, 7, 7}),
		(t_block_info){0,0,0,0});
	chunk->setBlock(BlockPos((int[4]){0, 8, 0, 7}),
		(t_block_info){LOG,0,0,0});
	chunk->setBlock(BlockPos((int[4]){0, 6, 0, 7}),
		(t_block_info){DIRT,0,0,0});
	chunk->setBlock(BlockPos((int[4]){0, 7, 0, 7}),
		(t_block_info){0,0,0,0});
}

void	WorldGenerator::genChunk(Chunk *chunk)
{
	(void)chunk;
}

void	WorldGenerator::configure(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
}