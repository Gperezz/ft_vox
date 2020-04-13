/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunck.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2020/04/13 20:00:53 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHUNCK_HPP
# define _CHUNCK_HPP

# include "Coords.hpp"
# include "Block.hpp"
# include "World.hpp"
# include <vector>

enum xz_vec {X, Z};
enum xyz_vec {X, Y, Z};
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 3>;

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

struct vbo_type {
	float tab[3];
	unsigned meta;
};

class Chunk{
	private:
		Block						blocks[16][16][16][16]; // [Mesh_index_y][Mesh_relative_x][Mesh_relative_y][mesh_relative_z]
		unsigned int				tabVao[16];
		unsigned int				tabVbo[16];
		std::vector<unsigned int>	valid;
		// unsigned int	ebo[16];
		ChunkPos			pos;
		Biome				biome;
		Geomorph			geomoprh;
		ChunkState			state;
		World				*world;
		bool				conditionValidate(std::vector<vbo_type> &tempVbo, int mesh_index, BlockPos posMesh, bool &b);
		void				generateVBO(void);
	public:
		Chunk(World*)
		Chunk(World*, ChunkPos);

		void			validateMesh(unsigned int mesh_index);
		void			validateChunk();

		Block&			get(BlockPos);
		Block&			get(int x, int y, int z);
		Block&			get(int i);
		Block&			operator[](BlockPos);
		Chunk			*getNeighboor(Direction);

		Block			*getBlockNeighboor(unsigned char , BlockPos, Direction);
		bool			blockSurrounded(std::vector<vbo_type> &tempVbo,int mesh_index, BlockPos posMesh)
};
#endif