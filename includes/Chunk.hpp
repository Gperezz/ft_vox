/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2020/04/15 16:04:42 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHUNK_HPP_
# define _CHUNK_HPP_

# include "World.hpp"
# include "Coords.hpp"
# include "Block.hpp"

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
	BlockPos({-LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, LENGTH_BLOCK / 2}),
	BlockPos({-LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, LENGTH_BLOCK / 2}),
	BlockPos({LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, LENGTH_BLOCK / 2}),
	BlockPos({LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, LENGTH_BLOCK / 2}),
	// -Z
	BlockPos({-LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2}),
	BlockPos({-LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2}),
	BlockPos({LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2}),
	BlockPos({LENGTH_BLOCK / 2, LENGTH_BLOCK / 2, -LENGTH_BLOCK / 2}),
};

struct s_direction_consts{
	BlockPos	block_vec;
	ChunkPos	chunk_vec;
	xyz_vec		axis;
	int			sens;
	BlockPos	pts[6];

}
static const struct s_direction_consts g_dir_c[] = {
	// NORTH
	{{0,0,0,1}, {0, 1}, Z, 1,
		{
		t_cube_pt[0], t_cube_pt[1], t_cube_pt[2],
		t_cube_pt[0], t_cube_pt[2], t_cube_pt[3],
		}
	},
	// WEST
	{{0,1,0,0}, {1, 0}, X, 1,
		{
		t_cube_pt[7], t_cube_pt[6], t_cube_pt[3],
		t_cube_pt[7], t_cube_pt[3], t_cube_pt[4],
		}
	},
	// SOUTH
	{{0,0,0,-1}, {0, -1}, Z, 0,
		{
		t_cube_pt[4], t_cube_pt[5], t_cube_pt[6],
		t_cube_pt[4], t_cube_pt[6], t_cube_pt[7],
		}
	},
	// EAST
	{{0,-1,0,0}, {-1, 0}, X, 0,
		{
		t_cube_pt[0], t_cube_pt[1], t_cube_pt[5],
		t_cube_pt[0], t_cube_pt[5], t_cube_pt[4],
		}
	},
	// UP
	{{0,0,1,0}, {0, 0}, Y, 1,
		{
		t_cube_pt[0], t_cube_pt[4], t_cube_pt[7],
		t_cube_pt[0], t_cube_pt[7], t_cube_pt[3],
		}
	},
	// DOWN
	{{0,0,-1,0}, {0, 0}, Y, 0,
		{
		t_cube_pt[1], t_cube_pt[5], t_cube_pt[6],
		t_cube_pt[1], t_cube_pt[6], t_cube_pt[2],
		}
	}

}

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
		std::vector<char>			valid;
		// unsigned int	ebo[16];
		ChunkPos					pos;
		Biome						biome;
		Geomorph					geomoprh;
		ChunkState					state;
		World						*world;
		bool						conditionValidate(std::vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh, bool &b);
		void						generateGraphics(void);
		void						generateVbo(char index, std::vector<vbo_type> tempVbo);
		void						deleteVbo(char index);
	public:
		Chunk(World*);
		Chunk(World*, ChunkPos);

		void						validateMesh(char meshIdx);
		void						validateChunk(void);

		Block&						get(BlockPos);
		Block&						get(int my, int x, int y, int z);
		Block&						get(int i);
		Block&						operator[](BlockPos);
		Chunk						*getNeighboor(Direction);

		Block						*getBlockNeighboor(BlockPos, Direction);
		bool						blockSurrounded(std::vector<vbo_type> &tempVbo, BlockPos posMesh)
};
#endif