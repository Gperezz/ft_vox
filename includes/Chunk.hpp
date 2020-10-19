/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2020/10/19 18:42:47 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHUNK_HPP_
# define _CHUNK_HPP_

# include "Block.hpp"
# include "Engine.hpp"
# include <vector>
# include <map>

class World;

enum Biome : char{
	PLAIN,
	FOREST,
	TAIGA,
	OCEAN,
	DESERT,
	JUNGLE
};

enum Geomorph : char{
	FLAT,
	HILLS,
	MOUNTAINS,
	PLATEAU,
	VOLCANO,
	AMPLIFIED
};

enum Direction : char{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	UP,
	DOWN
};

enum ChunkState : char{
	UNFENCED,
	FENCED
};

class Chunk{
	private:
		Block							blocks[16][16][16][16]; // [meshIdx_y][Mesh_relative_x][Mesh_relative_y][mesh_relative_z]
		unsigned int					tabVao[16];
		unsigned int					tabVbo[16];
		std::map<char, unsigned int>	valid;
		// unsigned int	ebo[16];
		ChunkPos						pos;
		// Biome						biome;
		// Geomorph						geomoprh;
		ChunkState						state;
		World							*world;
		bool							canPrintBlock(std::vector<vbo_type> &tempVbo, BlockPos posInMesh);
		void							fillTempVbo(std::vector<vbo_type> &tempVbo, t_direction_consts dir_c, BlockPos posInMesh, unsigned char id);
		bool							conditionValidate(std::vector<vbo_type> &tempVbo, BlockPos posInMesh, bool &b);
		void							validateMesh(char meshIdx);
		void							generateVbo(char index, std::vector<vbo_type> tempVbo);
		void							deleteVbo(char index);
	public:
		Chunk();
		Chunk(World*);
		Chunk(World*, ChunkPos);
		Chunk(const Chunk& copy);

		void							printSlice(int z);
		
		Block&							getBlock(BlockPos);
		Block&							getBlock(int my, int x, int y, int z);
		void							setBlock(BlockPos, t_block_info);
		ChunkPos						getPos(void);
		bool							getFenced(void);
		void							updateFenced(int source);

		Chunk							*getNeighboor(Direction);
		Block							*getBlockNeighboor(BlockPos, Direction);

		void							generateGraphics(void);
		void							displayChunk(Engine &e);

		Block&							operator[](BlockPos);
		void							operator=(const Chunk	&copy);
};
#endif