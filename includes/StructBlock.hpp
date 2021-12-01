/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StructBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:41:33 by gperez            #+#    #+#             */
/*   Updated: 2021/12/01 23:52:46 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _STRUCT_BLOCK_HPP
# define _STRUCT_BLOCK_HPP

# include "Coords.hpp"

enum xz_vec {XZ_X, XZ_Z};
enum xyz_vec {MY, X, Y, Z};

using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

# define LENGTH_BLOCK 1

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
	short		axis;
	BlockPos	pts[6];
}				t_direction_consts;

# define NB_PTS_CUBE 36

static t_direction_consts	g_dir_c[] = {
	// NORTH
	{BlockPos((int[4]){0,0,0,1}), ChunkPos((int[2]){0, 1}), Z,
		{
			t_cube_pt[0], t_cube_pt[1], t_cube_pt[2],
			t_cube_pt[0], t_cube_pt[2], t_cube_pt[3],
		}
	},
	// WEST
	{BlockPos((int[4]){0,-1,0,0}), ChunkPos((int[2]){-1, 0}), -X,
		{
			t_cube_pt[4], t_cube_pt[5], t_cube_pt[1],
			t_cube_pt[4], t_cube_pt[1], t_cube_pt[0],
		}
	},
	// SOUTH
	{BlockPos((int[4]){0,0,0,-1}), ChunkPos((int[2]){0, -1}), -Z,
		{
			t_cube_pt[4], t_cube_pt[6], t_cube_pt[5],
			t_cube_pt[4], t_cube_pt[7], t_cube_pt[6],
		}
	},
	// EAST
	{BlockPos((int[4]){0,1,0,0}), ChunkPos((int[2]){1, 0}), X,
		{
			t_cube_pt[3], t_cube_pt[2], t_cube_pt[6],
			t_cube_pt[3], t_cube_pt[6], t_cube_pt[7],
		}
	},
	// UP
	{BlockPos((int[4]){0,0,1,0}), ChunkPos((int[2]){0, 0}), Y,
		{
			t_cube_pt[0], t_cube_pt[7], t_cube_pt[4],
			t_cube_pt[0], t_cube_pt[3], t_cube_pt[7],
		}
	},
	// DOWN
	{BlockPos((int[4]){0,0,-1,0}), ChunkPos((int[2]){0, 0}), -Y,
		{
			t_cube_pt[1], t_cube_pt[5], t_cube_pt[6],
			t_cube_pt[1], t_cube_pt[6], t_cube_pt[2],
		}
	}

};

struct	vbo_type {
	float			pos[3];
	float			normal[3];
	float			coords[2];
	int				meta;
};

typedef struct	s_add_pt {
	char		dir;
	BlockPos	pts[6];
}				t_add_pt;


#endif