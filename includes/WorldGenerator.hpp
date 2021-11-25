/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/30 16:22:18 by gperez            #+#    #+#             */
/*   Updated: 2021/11/19 11:27:14 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_GENERATOR_HPP
# define WORLD_GENERATOR_HPP
# include "Rand.hpp"
# include "Block.hpp"
# include "Chunk.hpp"
# include "Perlin.hpp"
# include "Cave.hpp"
# include <thread>
# include <mutex>

enum Biome : char{
	OCEAN,
	BEACH,
	FOREST,
	GRASSLAND,
	DESERT,
	MOUNTAIN,
};

class WorldGenerator{
	private:
		unsigned long	seed;
		PerlinNoise		tP;
		Biome			biome;
		void			genThreadChunk(Chunk *chunk, ChunkPos pos, int x);
	public:
		WorldGenerator();
		WorldGenerator(unsigned long* seed);

		void genTest(Chunk*);
		void genChunk(Chunk*);
		void configure(unsigned long* seed);
		int biomeHeight(ChunkPos pos, unsigned char biome, int x, int z);
		unsigned char blockColor(double moisure, double elevation, unsigned char *type);
};
#endif