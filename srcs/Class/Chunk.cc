/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/04/15 13:42:10 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"

using namespace std;

Chunk::Chunk(World *w) : state(UNFENCED), world(w)
{
}

Chunk::Chunk(World *w, ChunkPos pos) : state(UNFENCED), pos(pos), world(w)
{
}

static void	fillTempVbo(vector<vbo_type> &tempVbo, BlockPos pts[6], BlockPos posMesh, int id)
{
	int	iPt;

	posMesh.add(-8);
	iPt = 0;
	while (iPt < 6)
	{
		tempVbo.push_back((vbo_type){pts[iPt] + posMesh, id});
		iPt++;
	}
}

bool		Chunk::blockSurrounded(vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh)
{
	char	dir;
	int		i;

	i = NORTH;
	dir = 0;
	while (i < 6)
	{
		if (Chunk::getBlockNeighboor(meshIdx, posMesh, (Direction)i)->getInfo().id != 0)
		{
			dir += 1;
			fillTempVbo(tempVbo, g_add_pt[i].pts, posMesh,
				this->blocks[meshIdx][posMesh[X]][posMesh[Y]][posMesh[Z]].getInfo().id);
		}
		i++;
	}
	return (dir == 6);
}

bool		Chunk::conditionValidate(vector<vbo_type> &tempVbo, char meshIdx, BlockPos posMesh, bool &b)
{
	if (this->blocks[meshIdx][pos[0]][pos[1]][pos[2]].getInfo().id == AIR
		|| this->blockSurrounded(tempVbo, meshIdx, posMesh))
		return (0);
	b = 1;
}

void		Chunk::generateVbo(char index, vector<vbo_type> tempVbo)
{
	glGenVertexArrays(1, &(Chunk::tabVao[index]));
	glBindVertexArray(tabVao[index]);
	glGenBuffers(1, &(Chunk::tabVbo[index]));
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
	glDeleteBuffers(1, &(Chunk::tabVbo[index]));
	glDeleteVertexArrays(1, &(Chunk::tabVao[index]));
}

void		Chunk::validateMesh(char meshIdx)
{
	BlockPos				pos; // [x][y][z]
	bool					validateValue;
	vector<vbo_type>		tempVbo;

	if (std::find(Chunk::valid.begin(), Chunk::valid.end(),
		meshIdx) != Chunk::valid.end())
		deleteVbo(meshIdx);
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

Block		*Chunk::getBlockNeighboor(BlockPos pos, Direction dir)
{
	if (dir == UP)
	{
		if (pos[Z] == 15)
		{
			if (pos[MY] == 15)
				return NULL;
			return tmp->get(pos[MY] + 1, pos[X], 0, pos[Z]);
		}
		return this->get(pos + (BlockPos){0, 0, 1, 0});
	}
	if (dir == UP)
	{
		if (pos[Z] == 15)
		{
			if (pos[MY] == 0)
				return NULL;
			return tmp->get(pos[MY] - 1, pos[X], 15, pos[Z]);
		}
		return this->get(pos + (BlockPos){0, 0, -1, 0});
	}
	if (dir == NORTH)
	{
		if (pos[Z] == 15)
		{
			// to add: overflow +z condition
			Chunk *tmp = this->world->[{this->pos[XZ_X], this->pos[XZ_Z] + 1}];
			return tmp->get(pos[MY], pos[X], pos[Y], 0);
		}
		return this->get(pos + (BlockPos){0, 0, 0, 1});
	}
	if (dir == WEST)
	{
		if (pos[X] == 15)
		{
			// to add: overflow +x condition

			Chunk *tmp = this->world->[{this->pos[XZ_X] + 1, this->pos[XZ_Z]}];
			return tmp->get(pos[MY], 0, pos[Y], pos[Z]);
		}
		return this->get(pos + (BlockPos){0, 1, 0, 0});
	} 
	if (dir == SOUTH)
	{
		if (pos[Z] == 0)
		{
			// to add: overflow -z condition
			Chunk *tmp = this->world->[{this->pos[XZ_X], this->pos[XZ_Z] - 1}];
			return tmp->get(pos[MY], pos[X], pos[Y], 15);
		}
		return this->get(pos + (BlockPos){0, 0, 0, -1});
	}
	if (dir == EAST)
	{
		if (pos[X] == 15)
		{
			// to add: overflow -x condition
			Chunk *tmp = this->world->[{this->pos[XZ_X] - 1, this->pos[XZ_Z]}];
			return tmp->get(pos[MY], 15, pos[Y], pos[Z]);
		}
		return this->get(pos + (BlockPos){0, -1, 0, 0});
	} 
}

Block&		Chunk::operator[](BlockPos pos)
{
	return this->get(pos);
}

Block&		Chunk::get(BlockPos pos)
{
	return this->get(pos[MY], pos[X], pos[Y], pos[Z]);
}

Block&		Chunk::get(int my, int x, int y, int z)
{
	return this->blocks[my][x][y][z];
}