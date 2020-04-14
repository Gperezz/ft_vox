/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunck.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2020/04/14 02:57:15 by gperez           ###   ########.fr       */
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

# define LENGTH_BLOCK 1

typedef struct	s_add_pt {
	char		dir;
	BlockPos	pts[6];
}				t_add_pt;

static BlockPos t_cube_pt[] = {
	// arriere // haut gauche -> bas gauche -> bas droite -> haut droite
	{-LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, LENGTH_BLOCK / 2},
	{-LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, LENGTH_BLOCK / 2},
	{LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, LENGTH_BLOCK / 2},
	{LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, LENGTH_BLOCK / 2},
	//avant
	{-LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2},
	{-LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2},
	{LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2},
	{LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2},
};

static t_add_pt	g_add_pt[] = {
	{NORTH, {
		t_cube_pt[0], t_cube_pt[1], t_cube_pt[2],
		t_cube_pt[0], t_cube_pt[2], t_cube_pt[3],
		}
	},
	{EAST, {
		t_cube_pt[7], t_cube_pt[6], t_cube_pt[3],
		t_cube_pt[7], t_cube_pt[3], t_cube_pt[4],
		}
	},
	{SOUTH, {
		t_cube_pt[4], t_cube_pt[5], t_cube_pt[6],
		t_cube_pt[4], t_cube_pt[6], t_cube_pt[7],
		}
	},
	{WEST, {
		t_cube_pt[0], t_cube_pt[1], t_cube_pt[5],
		t_cube_pt[0], t_cube_pt[5], t_cube_pt[4],
		}
	}
	{UP, {
		t_cube_pt[0], t_cube_pt[4], t_cube_pt[7],
		t_cube_pt[0], t_cube_pt[7], t_cube_pt[3],
		}
	}
	{DOWN, {
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
	float tab[3];
	unsigned meta;
};

class Chunk{
	private:
		Block						blocks[16][16][16][16]; // [meshIdx_y][Mesh_relative_x][Mesh_relative_y][mesh_relative_z]
		unsigned int				tabVao[16];
		unsigned int				tabVbo[16];
		std::vector<unsigned int>	valid;
		// unsigned int	ebo[16];
		ChunkPos					pos;
		Biome						biome;
		Geomorph					geomoprh;
		ChunkState					state;
		World						*world;
		bool						conditionValidate(std::vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh, bool &b);
		void						generateGraphics(void);
		void						generateVbo(char index);
		void						deleteVbo(char index);
	public:
		Chunk(World*)
		Chunk(World*, ChunkPos);

		void						validateMesh(char meshIdx);
		void						validateChunk(void);

		Block&						get(BlockPos);
		Block&						get(int x, int y, int z);
		Block&						get(int i);
		Block&						operator[](BlockPos);
		Chunk						*getNeighboor(Direction);

		Block						*getBlockNeighboor(char, BlockPos, Direction);
		bool						blockSurrounded(std::vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh)
};
#endif