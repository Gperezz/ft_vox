/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/04/14 02:57:13 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunck.hpp"

Chunk::Chunk(World *w) : state(UNFENCED), world(w)
{
}

Chunk::Chunk(World *w, ChunkPos pos) : state(UNFENCED), pos(pos), world(w)
{
}

static void	fillTempVbo(std::vector<vbo_type> &tempVbo, BlockPos pts[6], int id)
{
	int	iPt;

	iPt = 0;
	while (iPt < 6)
	{
		tempVbo.push_back((vbo_type){pts[iPt], id});
		iPt++;
	}
}

bool		Chunk::blockSurrounded(std::vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh)
{
	char	dir;
	int		i;

	i = NORTH;
	dir = 0;
	while (i < 6)
	{
		if (Chunk::getBlockNeighboor(meshIdx, posMesh, i).getInfo().id != 0)
		{
			dir += 1 << i;
			fillTempVbo(tempVbo, g_add_pt[i].pts, this->blocks[meshIdx][posMesh[X]][posMesh[Y]][posMesh[Z]]);
		}
		i++;
	}
	return (dir);
}

bool		Chunk::conditionValidate(std::vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh, bool &b)
{
	if (this->blocks[meshIdx][pos[0]][pos[1]][pos[2]].getInfo().id == AIR
		|| this->blockSurrounded(tempVbo, meshIdx, posMesh))
		return (0);
	b = 1;
}

void		Chunk::generateVbo(char index, std::vector<vbo_type> tempVbo)
{
	glGenVertexArrays(1, tabVao[index]);
	glBindVertexArray(tabVao[index]);
	glGenBuffers(1, tabVbo[index]);
	glBindBuffer(GL_ARRAY_BUFFER, tabVbo[index]);
	glBufferData(GL_ARRAY_BUFFER, tempVbo.size() * sizeof(vbo_type), tempVbo, GL_STATIC_DRAW);
	// glGenBuffers(1, &ebo);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EBO),
		// EBO, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(int), (void*)0);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(float) * 3 + sizeof(int), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void		Chunk::deleteVbo(char index)
{
	glDeleteBuffers(tabVbo[index]);
	glDeleteVertexArrays(tabVao[index]);
}

void		Chunk::validateMesh(char meshIdx)
{
	BlockPos				pos; // [x][y][z]
	bool					validateValue;
	std::vector<vbo_type>	tempVbo;

	deleteVbo(meshIndex);
	validateValue = 0;
	while (pos[X] < 16)
	{
		while (pos[Y] < 16)
		{
			while (pos[Z] < 16)
			{
				conditionValidate(tempVbo, meshIdx, pos, validateValue);
				pos[Z]++;
			}
			pos[Y]++;
		}
		pos[X]++;
	}
	if (validateValue)
	{
		generateVbo(meshIdx, tempVbo);
		this->valid.push_back(meshIdx);
	}
}

void		Chunk::validateChunk(void)
{
	this->valid.clear();
	for (unsigned i = 0; i < 16; i++)
		validateMesh(i);
}

void		Chunk::generateGraphics(void)
{
	unsigned int	i;

	i = 0;
	if (Chunk::state == UNFENCED)
		return ;
	Chunk::validateChunk();
}

Chunk		*Chunk::getNeighboor(Direction dir)
{
	if (dir == NORTH)
		return (world->[this->pos + ChunkPos({0, 1})]);
	else if (dir == EAST)
		return (world->[this->pos + ChunkPos({1, 0})]);
	else if (dir == SOUTH)
		return (world->[this->pos + ChunkPos({0, -1})]);
	else if (dir == WEST)
		return (world->[this->pos + ChunkPos({-1, 0})]);
	return this;
}

Block		*Chunk::getBlockNeighboor(char meshIdx, BlockPos pos, Direction dir)
{
	// de la merde pour test
	return (Chunk::blocks[meshIdx][pos[X]][pos[Y][pos[Z]]]);
}