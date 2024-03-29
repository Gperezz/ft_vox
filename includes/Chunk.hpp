/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2021/11/26 13:00:04 by gperez           ###   ########.fr       */
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
		Block							blocks[16][16][16][16]; // [meshIdx_y][Mesh_relative_x][Mesh_relative_y][mesh_relative_z]
		unsigned int					tabVao[16];
		unsigned int					tabVbo[16];
		unsigned int					tabVaoWater[16];
		unsigned int					tabVboWater[16];
		std::map<char, unsigned int>	valid;
		std::map<char, unsigned int>	validTrans;
		ChunkPos						pos;
		bool							generate;
		ChunkState						state;
		World							*world;
		void							canPrintBlockLoop(std::vector<vbo_type> &tempVbo, std::vector<vbo_type> &tempVboTrans, BlockPos posInMesh, int &i, char &dir, bool &isTrans);
		bool							canPrintBlock(std::vector<vbo_type> &tempVbo, std::vector<vbo_type> &tempVboTrans, BlockPos posInMesh, bool &isTrans);
		void							fillTempVbo(std::vector<vbo_type> &tempVbo, t_direction_consts dir_c, BlockPos posInMesh, t_id id);
		void							conditionValidate(std::vector<vbo_type> &tempVbo, std::vector<vbo_type>	&tempVboTrans, BlockPos posInMesh, bool &b, bool &bT);
		void							validateMesh(char meshIdx);
		void							generateVbo(char index, std::vector<vbo_type> tempVbo, bool isT);
		void							deleteVbos(void);
	public:
		Chunk();
		Chunk(World*);
		Chunk(World*, ChunkPos);
		Chunk(const Chunk& copy);
		~Chunk();
		bool							isGenerated(void);
		void							printSlice(int z);
		
		Block&							getBlock(BlockPos);
		Block&							getBlock(int my, int x, int y, int z);
		void							setBlock(BlockPos, t_block_info);
		void							setBlock(BlockPos blockPos, t_block_info info, unsigned char biome);
		unsigned char					getBiome(BlockPos blockPos);
		ChunkPos						getPos(void);
		bool							getFenced(void);
		void							updateFenced(int source);
		void							updateFencedUnsafe(int source);
		void							setUnfenced(void);
		void							updateDelFenced(void);

		Chunk							*getNeighboor(Direction);
		Chunk							*getNeighboorUnsafe(Direction dir);
		Block							*getBlockNeighboor(BlockPos, Direction);

		void							generateGraphics(void);
		void							generateGraphics(unsigned int mesh);
		void							displayChunk(Camera cam, Shader shader, Textures *t);
		void							displayChunkTransparency(Camera cam, Shader shader, Textures *t);

		Block&							operator[](BlockPos);
		void							operator=(const Chunk	&copy);
};
#endif