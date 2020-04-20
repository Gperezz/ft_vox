/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/04/20 03:53:34 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "World.hpp"

using namespace std;

Chunk::Chunk()
{
	this->state = UNFENCED;
	bzero(this->blocks, sizeof(this->blocks));
}

Chunk::Chunk(World *w)
{
	this->state = UNFENCED,
	this->world = w;
	bzero(this->blocks, sizeof(this->blocks));
}

Chunk::Chunk(World *w, ChunkPos pos)
{
	this->state = UNFENCED,
	this->pos = pos;
	this->world = w;
	bzero(this->blocks, sizeof(this->blocks));
}

Chunk::Chunk(const Chunk& copy)
{
	this->operator=(copy);
}

void		Chunk::operator=(const Chunk& copy)
{
	this->state = copy.state;
	this->pos = copy.pos;
	this->world = copy.world;
	memcpy(this->blocks, (void*)copy.blocks, sizeof(copy.blocks));
}

static void	fillTempVbo(vector<vbo_type> &tempVbo, BlockPos pts[6], BlockPos posMesh, int id)
{
	int			iPt;
	vbo_type	vboType;

	iPt = 0;
	while (iPt < 6)
	{
		vboType.tab[0] = pts[iPt].get(X) + posMesh.get(X);
		vboType.tab[1] = pts[iPt].get(Y) + posMesh.get(Y);
		vboType.tab[2] = pts[iPt].get(Z) + posMesh.get(Z);
		vboType.meta = id;
		tempVbo.push_back(vboType);
		iPt++;
	}
}

bool		Chunk::canPrintBlock(vector<vbo_type> &tempVbo, BlockPos posMesh)
{
	char	dir;
	int		i;

	i = NORTH;
	dir = 0;
	while (i < 6)
	{
		Block *tmp = this->getBlockNeighboor(posMesh, (Direction)i);
		if (!tmp || (tmp && tmp->getInfo().id == 0))
		{
			dir += 1;
			fillTempVbo(tempVbo, (BlockPos*)g_dir_c[i].pts, posMesh,
				this->getBlock(posMesh).getInfo().id);
		}
		i++;
	}
	ft_printf(YELLOW"dir %d\n" NA, dir);
	return (dir != 0);
}

bool		Chunk::conditionValidate(vector<vbo_type> &tempVbo, BlockPos posMesh, bool &b)
{
	if (this->getBlock(posMesh).getInfo().id == AIR
		|| !this->canPrintBlock(tempVbo, posMesh))
		return (0);
	b = 1;
	return (1);
}

void		Chunk::generateGraphics(void)
{
	if (Chunk::state == UNFENCED)
		return ;
	Chunk::validateChunk();
}

void		Chunk::generateVbo(char index, vector<vbo_type> tempVbo)
{
	glGenVertexArrays(1, &(Chunk::tabVao[(int)index]));
	glBindVertexArray(tabVao[(int)index]);
	glGenBuffers(1, &(Chunk::tabVbo[(int)index]));
	glBindBuffer(GL_ARRAY_BUFFER, tabVbo[(int)index]);
	glBufferData(GL_ARRAY_BUFFER, tempVbo.size() * sizeof(vbo_type), &tempVbo, GL_STATIC_DRAW);
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
	glDeleteBuffers(1, &(Chunk::tabVbo[(int)index]));
	glDeleteVertexArrays(1, &(Chunk::tabVao[(int)index]));
}

void		Chunk::validateMesh(char meshIdx)
{
	BlockPos				pos; // [meshY][x][y][z]
	bool					validateValue;
	vector<vbo_type>		tempVbo;

	if (this->valid.find(meshIdx) != Chunk::valid.end())
	{
		this->valid.erase(meshIdx);
		deleteVbo(meshIdx);
	}
	validateValue = 0;
	pos[MY] = meshIdx;
	while (pos[X] < 16)
	{
		while (pos[Y] < 16)
		{
			while (pos[Z] < 16)
			{
				conditionValidate(tempVbo, pos, validateValue);
				pos[Z]++;
			}
			pos[Y]++;
		}
		pos[X]++;
	}
	if (validateValue)
	{
		generateVbo(meshIdx, tempVbo);
		ft_printf(GREEN "generate VBO (Mesh %d) %d \n" NA, meshIdx, tempVbo.size() * 9);
		this->valid.insert({meshIdx, tempVbo.size() * 9});
	}
}

void		Chunk::validateChunk(void)
{
	ft_printf(ORANGE "Validating Chunk %d %d\n" NA, this->pos.get(0), this->pos.get(1));
	for (unsigned i = 0; i < 16; i++)
	{
		ft_printf(ORANGE "Validating Mesh %d\n" NA, i);
		validateMesh(i);
	}
}

void		Chunk::displayChunk(Engine &e)
{
	std::map<char, unsigned int>::iterator	it = this->valid.begin();
	Shader&									shader(e.getShader());

	ft_printf(RED "%d %u\n" NA, it->first, it->second);
	// ft_printf(YELLOW"%d %d\n" NA, this->getPos().get(0), this->getPos().get(1));
	while (it != this->valid.end())
	{
		// ft_printf(CYAN"%d\n" NA, it->first);
		glBindVertexArray(this->tabVao[(int)it->first]);
		glUseProgram(shader.getProgram());
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"view"), 1, GL_FALSE, glm::value_ptr(e.getCam().getMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"projection"), 1, GL_FALSE, glm::value_ptr(e.getCam().getProjMatrix()));
		glDrawArrays(GL_TRIANGLES, 0, it->second);
		it++;
	}
}

Chunk		*Chunk::getNeighboor(Direction dir)
{
	if (g_dir_c[dir].axis == Y)
		return this;
	return this->world->get(this->pos + g_dir_c[dir].chunk_vec);
}

Block		*Chunk::getBlockNeighboor(BlockPos pos, Direction dir)
{
	struct s_direction_consts&	c(g_dir_c[dir]);

	if (pos[c.axis] == c.sens ? 15 : 0)
	{
		if (c.axis == Y)
		{
			if (pos[MY] == c.sens ? 15 : 0)
				return NULL;
			pos[MY] += c.sens ? 1 : -1;
			pos[Y] = c.sens ? 0 : 15;
			return &this->getBlock(pos);
		}
		else
		{
			pos[c.axis] = c.sens ? 0 : 15;
			return &this->getNeighboor(dir)->getBlock(pos);
		}
	}
	return &this->getBlock(pos + c.block_vec);
}

Block&		Chunk::operator[](BlockPos pos)
{
	return this->getBlock(pos);
}

Block&		Chunk::getBlock(BlockPos pos)
{
	return this->getBlock(pos[MY], pos[X], pos[Y], pos[Z]);
}

Block&		Chunk::getBlock(int my, int x, int y, int z)
{
	return this->blocks[my][x][y][z];
}

void		Chunk::setBlock(BlockPos blockPos, t_block_info info)
{
	this->blocks[blockPos.get(0)][blockPos.get(1)][blockPos.get(2)][blockPos.get(3)] = info;
}

ChunkPos	Chunk::getPos(void)
{
	return this->pos;
}

void		Chunk::updateFenced(void)
{
	if (this->getNeighboor(NORTH) && this->getNeighboor(SOUTH) && this->getNeighboor(EAST) && this->getNeighboor(WEST))
		this->state = FENCED;
	else
		this->state = UNFENCED;
}

void		Chunk::printSlice(int z)
{
	BlockPos meshPos;

	meshPos[Z] = z;
	while (meshPos[Y] < 16)
	{
		while (meshPos[X] < 16)
		{
			printf(BLUE "%d " NA, this->getBlock(meshPos).getInfo().id);
			meshPos[X]++;
		}
		printf("\n");
		meshPos[Y]++;
		meshPos[X] = 0;
	}
	printf("\n");
}