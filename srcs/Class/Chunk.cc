/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/04/13 20:18:18 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunck.hpp"

Chunk::Chunk(World *w) : state(UNFENCED), world(w)
{
}

Chunk::Chunk(World *w, ChunkPos pos) : state(UNFENCED), pos(pos), world(w)
{
}

void	Chunk::validateChunk()
{
	for (unsigned i = 0; i < 16; i++)
		validateMesh(i);
}

bool	Chunk::blockSurrounded(std::vector<vbo_type> &tempVbo, int mesh_index, BlockPos posMesh)
{
	//remplissance du tempVbo
}

bool	Chunk::conditionValidate(std::vector<vbo_type> &tempVbo, int mesh_index, BlockPos posMesh, bool &b)
{
	if (this->blocks[mesh_index][pos[0]][pos[1]][pos[2]].getInfo().id == AIR
		|| this->blockSurrounded(tempVbo, mesh_index, posMesh))
		return (0);
	b = 1;
}

void		Chunk::validateMesh(unsigned int mesh_index)
{
	BlockPos				pos; // [x][y][z]
	bool					validateValue;
	std::vector<vbo_type>	tempVbo;

	validateValue = 0;
	while (pos[X] < 16)
	{
		while (pos[Y] < 16)
		{
			while (pos[Z] < 16)
			{
				conditionValidate(tempVbo, mesh_index, pos, validateValue);
				pos[Z]++;
			}
			pos[Y]++;
		}
		pos[X]++;
	}
	if (validateValue)
	{
		//generateVbo
		this->valid.push_back(mesh_index);
	}
}

void	Chunk::generateVBO(void)
{
	unsigned int	i;

	i = 0;
	if (Chunk::state == UNFENCED)
		return ;
	while (i < Chunk::valid.size())
	{
		
		i++;
	}
}

Chunk	*Chunk::getNeighboor(Direction dir)
{
	if (dir == NORTH)
		return (world->[this->pos + ChunkPos({0, 1})]);
	else if (dir == EAST)
		return (world->[this->pos + ChunkPos({1, 0})]);
	else if (dir == NORTH)
		return (world->[this->pos + ChunkPos({0, -1})]);
	else if (dir == NORTH)
		return (world->[this->pos + ChunkPos({-1, 0})]);
	return this;
}
b