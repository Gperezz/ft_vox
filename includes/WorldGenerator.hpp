/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/30 16:22:18 by gperez            #+#    #+#             */
/*   Updated: 2021/11/18 17:23:40 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_GENERATOR_HPP
# define WORLD_GENERATOR_HPP
# include "Rand.hpp"
# include "Block.hpp"
# include "Chunk.hpp"
# include "Perlin.hpp"
# include "Cave.hpp"

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
	public:
		WorldGenerator();
		WorldGenerator(unsigned long* seed);

		void genTest(Chunk*);
		void genChunk(Chunk*);
		void configure(unsigned long* seed);
		int biomeHeight(ChunkPos pos, unsigned char biome, int x, int z);
		unsigned char blockColor(double moisure, double elevation);
};
#endif