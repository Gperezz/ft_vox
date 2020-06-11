/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/05/08 19:48:51 by gperez           ###   ########.fr       */
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

static void	fillTempVbo(vector<vbo_type> &tempVbo, t_direction_consts dir_c, BlockPos posMesh, int id)
{
	int			iPt;
	vbo_type	vboType;

	iPt = 0;
	while (iPt < 6)
	{
		vboType.tab[0] = dir_c.pts[iPt].get(X) + posMesh.get(X);
		vboType.tab[1] = dir_c.pts[iPt].get(Y) + posMesh.get(Y);
		vboType.tab[2] = dir_c.pts[iPt].get(Z) + posMesh.get(Z);
		// ft_printf(MAGENTA "X:%d Y:%d Z:%d\n" NA, posMesh.get(X), posMesh.get(Y), posMesh.get(Z));
		// ft_printf(CYAN "X:%f Y:%f Z:%f\n" NA, vboType.tab[0], vboType.tab[1], vboType.tab[2]);
		vboType.meta = dir_c.axis;
		(void)id;
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
			dir += 1 << i;
			fillTempVbo(tempVbo, (t_direction_consts)g_dir_c[i], posMesh,
				this->getBlock(posMesh).getInfo().id);
		}
		i++;
	}
	ft_printf(YELLOW"dir %b\n" NA, dir);
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

void		Chunk::generateVbo(char index, vector<vbo_type> tempVbo)
{
	glGenVertexArrays(1, &(Chunk::tabVao[(int)index]));
	glBindVertexArray(tabVao[(int)index]);
	glGenBuffers(1, &(Chunk::tabVbo[(int)index]));
	glBindBuffer(GL_ARRAY_BUFFER, tabVbo[(int)index]);
	glBufferData(GL_ARRAY_BUFFER, tempVbo.size() * sizeof(vbo_type), &tempVbo[0], GL_STATIC_DRAW);
	// glGenBuffers(1, &ebo);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EBO),
		// EBO, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(int), (void*)0);
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(float) * 3 + sizeof(int), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
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
		// ft_printf(BOLD_YELLOW "X = %d\n" NA, pos[X]);
		while (pos[Y] < 16)
		{
			while (pos[Z] < 16)
			{
				conditionValidate(tempVbo, pos, validateValue);
				pos[Z]++;
			}
			pos[Z] = 0;
			pos[Y]++;
		}
		pos[Y] = 0;
		pos[X]++;
	}
	if (validateValue && tempVbo.size())
	{
		generateVbo(meshIdx, tempVbo);
		ft_printf(GREEN "generate VBO (Mesh %d) %d points (%d floats)\n" NA, meshIdx, tempVbo.size(), tempVbo.size() * 3);
		this->valid.insert({meshIdx, tempVbo.size()});
	}
}

void		Chunk::deleteVbo(char index)
{
	glDeleteBuffers(1, &(Chunk::tabVbo[(int)index]));
	glDeleteVertexArrays(1, &(Chunk::tabVao[(int)index]));
}

////////////////////////////// Public //////////////////////////////



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

bool	Chunk::getFenced(void)
{
	return (this->state);
}

void		Chunk::updateFenced(void)
{
	if (this->getNeighboor(NORTH) && this->getNeighboor(SOUTH) && this->getNeighboor(EAST) && this->getNeighboor(WEST))
		this->state = FENCED;
	else
		this->state = UNFENCED;
}

Chunk		*Chunk::getNeighboor(Direction dir)
{
	if (g_dir_c[dir].axis == Y)
		return NULL;
	return this->world->get(this->pos + g_dir_c[dir].chunk_vec);
}

Block		*Chunk::getBlockNeighboor(BlockPos pos, Direction dir) // Fonction peut etre opti
{
	struct s_direction_consts&	c(g_dir_c[dir]);
	Chunk						*neighboor;

	if (pos[c.axis] == 15 && c.block_vec[c.axis] == 1)
	{
		pos[c.axis] = 0;
		neighboor = this->getNeighboor(dir);
		if (neighboor)
			return &neighboor->getBlock(pos);
		return NULL;
	}
	else if (pos[c.axis] == 0 && c.block_vec[c.axis] == -1)
	{
		pos[c.axis] = 15;
		neighboor = this->getNeighboor(dir);
		if (neighboor)
			return &neighboor->getBlock(pos);
		return NULL;
	}
	return &this->getBlock(pos + c.block_vec);
}

void		Chunk::generateGraphics(void)
{
	ft_printf(ORANGE "Validating Chunk %d %d\n" NA, this->pos.get(0), this->pos.get(1));
	for (unsigned i = 0; i < 16; i++)
	{
		ft_printf(ORANGE "Validating Mesh %d\n" NA, i);
		validateMesh(i);
	}
}

void		Chunk::displayChunk(Engine &e, glm::mat4 world)
{
	std::map<char, unsigned int>::iterator	it = this->valid.begin();
	Shader&									shader(e.getShader());
	Textures								*t;

	t = e.getTexture(GROUND_TXT);
	while (it != this->valid.end())
	{
		// ft_printf(CYAN "%d %u\n" NA, it->first, it->second);
		glBindVertexArray(this->tabVao[(int)it->first]);
		glUseProgram(shader.getProgram());
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"world"), 1, GL_FALSE, glm::value_ptr(world));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"view"), 1, GL_FALSE, glm::value_ptr(e.getCam().getMatrix(true)));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"projection"), 1, GL_FALSE, glm::value_ptr(e.getCam().getProjMatrix()));
		glUniform1i(glGetUniformLocation(shader.getProgram(),
			"basicTexture"), t ? t->getTxt() : 0);
		glDrawArrays(GL_TRIANGLES, 0, it->second);
		it++;
	}
}

Block&		Chunk::operator[](BlockPos pos)
{
	return this->getBlock(pos);
}

void		Chunk::operator=(const Chunk& copy)
{
	this->state = copy.state;
	this->pos = copy.pos;
	this->world = copy.world;
	memcpy(this->blocks, (void*)copy.blocks, sizeof(copy.blocks));
}