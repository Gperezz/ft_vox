/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2020/11/24 16:39:10 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldGenerator.hpp"

WorldGenerator::WorldGenerator()
{
	this->seed = Rand<unsigned long>().generate();
	this->tP = PerlinNoise(16, this->seed);
	
}

WorldGenerator::WorldGenerator(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
	this->tP = PerlinNoise(16, this->seed);
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
						(t_block_info){2,0,0,0});
				}
			}
		}
	}
	chunk->setBlock(BlockPos((int[4]){7, 7, 7, 7}),
		(t_block_info){0,0,0,0});
	chunk->setBlock(BlockPos((int[4]){0, 7, 0, 7}),
		(t_block_info){0,0,0,0});
}

void	WorldGenerator::genChunk(Chunk *chunk)
{
	for (int x = 0; x < 16; x++){
		for (int z = 0; z < 16; z++){
			ChunkPos pos = chunk->getPos();
			float perlinNoise = tP(0.0f, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
			int height = (int)((perlinNoise + 1.0f) * 10) + 20;
			int y = 0;
			for (; y < height; y++) {
				chunk->setBlock(BlockPos((int[4]){y / 16, x, y % 16, z}),
						(t_block_info){2,0,0,0});
			}
		}
	}
}

void	WorldGenerator::configure(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
}