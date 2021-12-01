/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2021/11/30 13:53:11 by maiwenn          ###   ########.fr       */
/*   Updated: 2021/11/27 22:25:41 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldGenerator.hpp"

WorldGenerator::WorldGenerator()
{
	this->seed = Rand<unsigned long>().generate();
	
}

WorldGenerator::WorldGenerator(unsigned long* seed)
{
	this->seed = *seed;
}

unsigned char WorldGenerator::blockColor(double moisure, double *elevation, unsigned char *type)
{
    if (*elevation < 110.)
	{
		*type = WATER;
		*elevation += 10;
        return OCEAN;
	}
	if (*elevation < 111.)
    {
		*type = SAND;
		*elevation += 10;
	    return BEACH;
	}
    if (*elevation > 150.) 
    {
		*type = SNOW;
        if (moisure < 130.)
            *type = STONE;
		return MOUNTAIN;
    }
	if (*elevation < 121)
		*elevation = 121;
    if (*elevation < 150.)
    {
        if (moisure < 108.)
		{
			*type = SAND;
            return DESERT;
		}
		*type = GRASS;
        if (moisure < 120.)
			return GRASSLAND;
        else if (moisure < 140.)
			return FOREST;
    }
	*type = GRASS;
    return GRASSLAND;
}

double elevation(double x, double z, double seed)
{
	PerlinNoise noise = PerlinNoise(seed);
	double e = (5.00 * noise.perlin(1, 0.005, 8, x, z) 
		+ 4.50 * noise.perlin(1, 0.001, 1, 7 * x, 2 * z)
		+ 3.25 * noise.perlin(1, 0.005, 2, 3 * x, 3 * z)
		+ 2.13 * noise.perlin(1, 0.004, 6, 4 * x, 4 * z)
		+ 1.06 * noise.perlin(1, 0.03, 1, 5 * x, 5 * z)
	);
    e = e / (5.00 + 4.50 + 3.25 + 2.13 + 1.06);
    e = pow(e, 1.0);
    return (e);
}

void	putBlock(Chunk *chunk, unsigned char biome, unsigned char type, int x, int y, int z, double e)
{
	for (; y <= e; y++)
	{
		int pos[4] = {y / 16, x, y % 16, z};
		chunk->setBlock(BlockPos(pos), (t_block_info){type,0,0,0}, biome);
	}
}

void	chooseBlock(Chunk *chunk, unsigned char biome, unsigned char type, int x, int z, double e)
{
	if (type == WATER)
	{
		putBlock(chunk, biome, SAND, x, 0, z, e);
		putBlock(chunk, biome, WATER, x, e, z, 120);
	}
	else if (type == SNOW)
	{
		putBlock(chunk, biome, STONE, x, 0, z, e - 1);
		putBlock(chunk, biome, SNOW, x, e, z, e);
	}
	else if (type == GRASS)
	{
		putBlock(chunk, biome, STONE, x, 0, z, 2 * (e / 3));
		putBlock(chunk, biome, DIRT, x, 2 * (e / 3), z, e - 1);
		putBlock(chunk, biome, GRASS, x, e, z, e);
	}
	else
	{
		putBlock(chunk, biome, type, x, 0, z, e);
	}	
}

void	WorldGenerator::genChunk(Chunk *chunk)
{
	Cave cave = Cave();
	for (int x = 0; x < 16; x++){
		for (int z = 0; z < 16; z++){
			
			ChunkPos pos = chunk->getPos();
			double e = elevation(pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX, this->seed);
			double m = elevation(pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX, this->seed / 300);
			e = ((e + 1) / 2) * 255;
			m = ((m + 1) / 2) * 255;
			unsigned char type;
			unsigned char biome = blockColor(m, &e, &type);
			chooseBlock(chunk, biome, type, x, z, e);
		}
	}
	cave.createCave(chunk, this->seed / 500);
}

void	WorldGenerator::configure(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
}