/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2020/04/19 18:51:38 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHUNK_HPP_
# define _CHUNK_HPP_

# include "Coords.hpp"
# include "Block.hpp"
# include "Engine.hpp"
# include <vector>
# include <map>

class World;

enum xz_vec {XZ_X, XZ_Z};
enum xyz_vec {MY, X, Y, Z};

using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

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

# define LENGTH_BLOCK 1

typedef struct	s_add_pt {
	char		dir;
	BlockPos	pts[6];
}				t_add_pt;

static BlockPos t_cube_pt[] = {
	// +Z // haut gauche -> bas gauche -> bas droite -> haut droite
	BlockPos((int[4]){0,0				, LENGTH_BLOCK	, LENGTH_BLOCK}),
	BlockPos((int[4]){0,0				, 0				, LENGTH_BLOCK}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, 0				, LENGTH_BLOCK}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, LENGTH_BLOCK	, LENGTH_BLOCK}),
	// -Z
	BlockPos((int[4]){0,0				, LENGTH_BLOCK	, 0}),
	BlockPos((int[4]){0,0				, 0				, 0}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, 0				, 0}),
	BlockPos((int[4]){0,LENGTH_BLOCK	, LENGTH_BLOCK	, 0}),
};

typedef struct	s_direction_consts{
	BlockPos	block_vec;
	ChunkPos	chunk_vec;
	xyz_vec		axis;
	bool		sens;
	BlockPos	pts[6];
}				t_direction_consts;

static t_direction_consts	g_dir_c[] = {
	// NORTH
	{BlockPos((int[4]){0,0,0,1}), ChunkPos((int[2]){0, 1}), Z, 1,
		{
			t_cube_pt[0], t_cube_pt[1], t_cube_pt[2],
			t_cube_pt[0], t_cube_pt[2], t_cube_pt[3],
		}
	},
	// WEST
	{BlockPos((int[4]){0,1,0,0}), ChunkPos((int[2]){1, 0}), X, 1,
		{
			t_cube_pt[7], t_cube_pt[6], t_cube_pt[3],
			t_cube_pt[7], t_cube_pt[3], t_cube_pt[4],
		}
	},
	// SOUTH
	{BlockPos((int[4]){0,0,0,-1}), ChunkPos((int[2]){0, -1}), Z, 0,
		{
			t_cube_pt[4], t_cube_pt[5], t_cube_pt[6],
			t_cube_pt[4], t_cube_pt[6], t_cube_pt[7],
		}
	},
	// EAST
	{BlockPos((int[4]){0,-1,0,0}), ChunkPos((int[2]){-1, 0}), X, 0,
		{
			t_cube_pt[0], t_cube_pt[1], t_cube_pt[5],
			t_cube_pt[0], t_cube_pt[5], t_cube_pt[4],
		}
	},
	// UP
	{BlockPos((int[4]){0,0,1,0}), ChunkPos((int[2]){0, 0}), Y, 1,
		{
			t_cube_pt[0], t_cube_pt[4], t_cube_pt[7],
			t_cube_pt[0], t_cube_pt[7], t_cube_pt[3],
		}
	},
	// DOWN
	{BlockPos((int[4]){0,0,-1,0}), ChunkPos((int[2]){0, 0}), Y, 0,
		{
			t_cube_pt[1], t_cube_pt[5], t_cube_pt[6],
			t_cube_pt[1], t_cube_pt[6], t_cube_pt[2],
		}
	}

};

enum ChunkState : char{
	UNFENCED,
	FENCED
};

struct vbo_type {
	float		tab[3];
	unsigned	meta;
};

class Chunk{
	private:
		Block							blocks[16][16][16][16]; // [meshIdx_y][Mesh_relative_x][Mesh_relative_y][mesh_relative_z]
		unsigned int					tabVao[16];
		unsigned int					tabVbo[16];
		std::map<char, unsigned int>	valid;
		// unsigned int	ebo[16];
		ChunkPos						pos;
		// Biome							biome;
		// Geomorph						geomoprh;
		ChunkState						state;
		World							*world;
		bool							canPrintBlock(std::vector<vbo_type> &tempVbo, BlockPos posMesh);
		bool							conditionValidate(std::vector<vbo_type> &tempVbo, BlockPos posMesh, bool &b);
		void							validateMesh(char meshIdx);
		void							validateChunk(void);
		void							generateVbo(char index, std::vector<vbo_type> tempVbo);
		void							deleteVbo(char index);
	public:
		Chunk();
		Chunk(World*);
		Chunk(World*, ChunkPos);
		Chunk(const Chunk& copy);
		
		void							printSlice(int z);
		void							displayChunk(Engine &e);
		void							generateGraphics(void);

		Block&							getBlock(BlockPos);
		Block&							getBlock(int my, int x, int y, int z);
		ChunkPos						getPos(void);
		Block&							operator[](BlockPos);
		void							operator=(const Chunk	&copy);
		Chunk							*getNeighboor(Direction);

		Block							*getBlockNeighboor(BlockPos, Direction);

		void							updateFenced(void);
};
#endif