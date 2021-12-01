/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cc                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2021/11/26 12:18:12 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldGenerator.hpp"

static void		printBiome(unsigned char biome)
{
	if (biome == OCEAN)
		printf("ocean\n");
	if (biome == BEACH)
		printf("beach\n");
	if (biome == FOREST)
		printf("forest\n");
	if (biome == GRASSLAND)
		printf("grassland\n");
	if (biome == DESERT)
		printf("desert\n");
	if (biome == MOUNTAIN)
		printf("mountain\n");
}

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

unsigned char WorldGenerator::blockColor(double moisure, double elevation, unsigned char *type)
{
    if (elevation < 110)
	{
		*type = WATER;
        return OCEAN;
	}
	if (elevation < 111)
    {
		*type = SAND;
	    return BEACH;
	}
    if (elevation > 150) 
    {
        if (moisure < 130)
            *type = STONE;
        *type = SNOW;
		return MOUNTAIN;
    }
    if (elevation < 197)
    {
        if (moisure < 108)
		{
			*type = SAND;
            return DESERT;
		}
		*type = GRASS;
        if (moisure < 114)
			return GRASSLAND;
        else if (moisure < 76)
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
		// + 0.03 * noise.perlin(1, 0.01, 6, 6 * x, 6 * z)
	);
    e = e / (5.00 + 4.50 + 3.25 + 2.13 + 1.06 /*+ 0.03*/);
    e = pow(e, 1.0);
    return (e);
}

// double moisure(double x, double z, double seed)
// {
// 	PerlinNoise noise = PerlinNoise();
// 	double m = (1.00 * noise.perlin(1, 1.0, 3, x, z)
// 		+ 0.75 * noise.perlin(1, 1.0, 3, 2 * x, 2 * z)
// 		+ 0.33 * noise.perlin(1, 1.0, 3, 3 * x, 3 * z)
// 		+ 0.33 * noise.perlin(1, 1.0, 3, 4 * x, 4 * z)
// 		+ 0.33 * noise.perlin(1, 1.0, 3, 5 * x, 5 * z)
// 		+ 0.50 * noise.perlin(1, 1.0, 3, 6 * x, 6 * z)
// 	);
//     m = m / (1.00 + 0.75 + 0.33 /*+ 0.33 + 0.33 + 0.50*/);
// 	return(m);
// }

void	putBlock(Chunk *chunk, unsigned char biome, unsigned char type, int x, int y, int z, double e)
{
	
	for (y; y < e; y++)
	{
		// printf("----NewBlock\n");
		// printBiome(biome);
		int pos[4] = {y / 16, x, y % 16, z};
		chunk->setBlock(BlockPos(pos), (t_block_info){type,0,0,0}, biome);
		// printBiome(chunk->getBiome(BlockPos((int[4]){y / 16, x, y % 16, z})));
	}
	
}

void	chooseBlock(Chunk *chunk, unsigned char biome, unsigned char type, int x, int z, double e)
{
	// printBiome(biome);
	if (type == WATER)
	{
		putBlock(chunk, biome, SAND, x, 0, z, e);
		putBlock(chunk, biome, WATER, x, e, z, 111);
	}
	else if (type == SNOW)
	{
		putBlock(chunk, biome, STONE, x, 0, z, e - 1);
		putBlock(chunk, biome, SNOW, x, e, z, e);
	}
	else if (type == GRASS)
	{
		putBlock(chunk, biome, STONE, x, 0, z, e / 2);
		putBlock(chunk, biome, DIRT, x, e / 2, z, e - 1);
		putBlock(chunk, biome, GRASS, x, e, z, e);
	}
	else
	{
		putBlock(chunk, biome, type, x, 0, z, e);
	}	
}

#define SEED_CAVE 6

void	WorldGenerator::genChunk(Chunk *chunk)
{
	Cave cave = Cave();
	ChunkPos pos = chunk->getPos();

	for (int x = 0; x < 16; x++){
		for (int z = 0; z < 16; z++){
			
			ChunkPos pos = chunk->getPos();
			double e = elevation(pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX, this->seed);
			double m = elevation(pos[0] * 16.0 + (float)x + (float)SHRT_MAX, pos[1] * 16.0 + (float)z + (float)SHRT_MAX, this->seed / 300);
			e = ((e + 1) / 2) * 255;
			m = ((m + 1) / 2) * 255;
			unsigned char type;
			unsigned char biome = blockColor(m, e, &type);
			// printBiome(biome);
			chooseBlock(chunk, biome, type, x, z, e);
		}
	}
	cave.createCave(chunk, SEED_CAVE);
}

void	WorldGenerator::configure(unsigned long* seed)
{
	if (!seed)
		this->seed = Rand<unsigned long>().generate();
	else
		this->seed = *seed;
}