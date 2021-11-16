/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2021/11/15 14:36:58 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldGenerator.hpp"

WorldGenerator::WorldGenerator()
{
	this->seed = Rand<unsigned long>().generate();
	this->tP = PerlinNoise(1);
	
}

WorldGenerator::WorldGenerator(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
	this->tP = PerlinNoise(this->seed);
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
		(t_block_info){LEAVES,0,0,0});
	chunk->setBlock(BlockPos((int[4]){0, 6, 0, 7}),
		(t_block_info){DIRT,0,0,0});
	chunk->setBlock(BlockPos((int[4]){0, 7, 0, 7}),
		(t_block_info){0,0,0,0});
	chunk->setBlock(BlockPos((int[4]){15, 7, 15, 7}),
		(t_block_info){0,0,0,0});
}

// int WorldGenerator::biomeHeight(ChunkPos pos, unsigned char biome, int x, int z)
// {
// 	PerlinNoise noise = PerlinNoise();
// 	int height;

// 	if (biome == WATER)
// 	{
// 		height = noise.perlin(1, 0.003, 3, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 4) * 255);
// 	}
// 	if (biome == BEACH)
// 	{
// 		height = noise.perlin(1, 0.003, 3, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 4) * 255);
// 	}
// 	if (biome == GRASS)
// 	{
// 		height = noise.perlin(1, 0.0003, 1, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 2) * 255);
		
// 	}
// 	if (biome == FOREST)
// 	{
// 		height = noise.perlin(1, 0.005, 1, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 2) * 255);
		
// 	}
// 	if (biome == STONE)
// 	{
// 		height = noise.perlin(1, 0.5, 3, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 2) * 255);
		
// 	}
// 	if (biome == SNOW)
// 	{
// 		height = noise.perlin(1, 0.5, 3, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 2) * 255);
		
// 	}
// 	if (biome == DESERT)
// 	{
// 		height = noise.perlin(1, 0.0005, 1, pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX);
// 		return(((height + 1) / 2) * 255);
		
// 	}


// }

unsigned char WorldGenerator::blockColor(double moisure, double elevation)
{
    if (elevation < 110)
	{
		this->biome = OCEAN;
        return WATER;
	}
	if (elevation < 111)
    {
		this->biome = BEACH;
	    return SAND;
	}
    if (elevation > 150) 
    {
		this->biome = MOUNTAIN;
        if (moisure < 130)
            return STONE;
        return SNOW;
    }
    if (elevation < 197)
    {
        if (moisure < 108)
		{
			this->biome = DESERT;
            return SAND;
		}
        if (moisure < 114)
			this->biome = GRASSLAND;
        else if (moisure < 76)
			this->biome = FOREST;
		return DIRT;
    }
	this->biome = GRASSLAND;
    return DIRT;
}
//sand water snow stone grass dirt forest

double elevation(double x, double z, double seed)
{
	PerlinNoise noise = PerlinNoise(seed);
	double e = (5.00 * noise.perlin(1, 0.005, 8, x, z) 
		+ 4.50 * noise.perlin(1, 0.001, 1, 7 * x, 2 * z)
		+ 3.25 * noise.perlin(1, 0.005, 2, 3 * x, 3 * z)
		+ 2.13 * noise.perlin(1, 0.004, 6, 4 * x, 4 * z)
		+ 1.06 * noise.perlin(1, 0.03, 1, 5 * x, 5 * z)
		// + 0.03 * noise.perlin(1, 0.01, 6, 6 * x, 6 * z)
	);
    e = e / (5.00 + 4.50 + 3.25 + 2.13 + 1.06 /*+ 0.03*/);
    e = pow(e, 1.0);
    return (e);
}

double moisure(double x, double z, double seed)
{
	PerlinNoise noise = PerlinNoise();
	double m = (1.00 * noise.perlin(1, 1.0, 3, x, z)
		+ 0.75 * noise.perlin(1, 1.0, 3, 2 * x, 2 * z)
		+ 0.33 * noise.perlin(1, 1.0, 3, 3 * x, 3 * z)
		+ 0.33 * noise.perlin(1, 1.0, 3, 4 * x, 4 * z)
		+ 0.33 * noise.perlin(1, 1.0, 3, 5 * x, 5 * z)
		+ 0.50 * noise.perlin(1, 1.0, 3, 6 * x, 6 * z)
	);
    m = m / (1.00 + 0.75 + 0.33 /*+ 0.33 + 0.33 + 0.50*/);
	return(m);
}

void	putBlock(Chunk *chunk, unsigned char type, int x, int y, int z, double e)
{
	for (y; y < e; y++)
	{
		chunk->setBlock(BlockPos((int[4]){y / 16, x, y % 16, z}),
				(t_block_info){type,0,0,0});
		}
}

void	chooseBlock(Chunk *chunk, unsigned char type, int x, int z, double e)
{
	if (type == WATER)
	{
		putBlock(chunk, SAND, x, 0, z, e);
		putBlock(chunk, WATER, x, e, z, 110);
	}
	else if (type == SNOW)
	{
		putBlock(chunk, STONE, x, 0, z, e - 1);
		putBlock(chunk, SNOW, x, e, z, e);
	}
	else
	{
		putBlock(chunk, type, x, 0, z, e);
	}	
}

void	WorldGenerator::genChunk(Chunk *chunk)
{
	// printf("x:\n");
	for (int x = 0; x < 16; x++){
		// printf("z: ");
		for (int z = 0; z < 16; z++){
			
			ChunkPos pos = chunk->getPos();
			double e = elevation(pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX, 1567612511);
			double m = elevation(pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX, 3);
			e = ((e + 1) / 2) * 255;
			m = ((m + 1) / 2) * 255;
			unsigned char type = blockColor(m, e);
			// printf("%f, \n", height);
			
			chooseBlock(chunk, type, x, z, e);
		}
		// printf("\n");
	}
	// printf("\n");
}

void	WorldGenerator::configure(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
}