/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/30 16:22:18 by gperez            #+#    #+#             */
/*   Updated: 2021/12/01 15:41:46 by gperez           ###   ########.fr       */
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

#define SEED 1567612511
#define MIN_SEED 15676125

class WorldGenerator{
	private:
		unsigned long	seed;
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