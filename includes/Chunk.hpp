/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2021/11/03 17:55:06 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHUNK_HPP_
# define _CHUNK_HPP_

# include "Block.hpp"
# include "StructBlock.hpp"
# include "Camera.hpp"
# include "Shader.hpp"
# include "Textures.hpp"
# include <vector>
# include <map>
# include <mutex>

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
	WEST,
	SOUTH,
	EAST,
	UP,
	DOWN
};

enum ChunkState : char{
	UNFENCED,
	FENCED
};

class Chunk{
	private:
		std::mutex						validMutex;
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
		~Chunk();

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
		void							generateGraphics(unsigned int mesh);
		void							displayChunk(Camera cam, Shader shader, Textures *t);

		Block&							operator[](BlockPos);
		void							operator=(const Chunk	&copy);
};
#endif