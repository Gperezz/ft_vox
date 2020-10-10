/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/10/10 18:36:00 by gperez           ###   ########.fr       */
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


void	Chunk::fillTempVbo(vector<vbo_type> &tempVbo, t_direction_consts dir_c, BlockPos posInMesh, unsigned char id)
{
	int			iPt;
	vbo_type	vboType;
	short		idBitwise;

	iPt = 0;
	while (iPt < 6)
	{
		vboType.tab[0] = dir_c.pts[iPt].get(X) + posInMesh.get(X) + this->getPos().get(0) * 16;
		vboType.tab[1] = dir_c.pts[iPt].get(Y) + posInMesh.get(Y) + posInMesh.get(MY) * 16;
		vboType.tab[2] = dir_c.pts[iPt].get(Z) + posInMesh.get(Z) + this->getPos().get(1) * 16;

		vboType.meta = dir_c.axis < 0 ? dir_c.axis + 7 : dir_c.axis;
		idBitwise = id << 8;
		vboType.meta = (int)vboType.meta | idBitwise;
		tempVbo.push_back(vboType);
		iPt++;
	}
	// ft_printf(RED"Direction : %d\n" NA, dir_c.axis);

}

bool		Chunk::canPrintBlock(vector<vbo_type> &tempVbo, BlockPos posInMesh)
{
	char	dir;
	int		i;

	i = NORTH;
	dir = 0;
	while (i < 6)
	{
		Block *tmp = this->getBlockNeighboor(posInMesh, (Direction)i);
		if (!tmp || (tmp->getInfo().id == AIR))
		{
			dir += 1 << i; //Faces visibles
			fillTempVbo(tempVbo, (t_direction_consts)g_dir_c[i], posInMesh,
				Textures::getIndexTxt((e_BlockType)this->getBlock(posInMesh).getInfo().id));
		}
		i++;
	}
	// ft_printf(YELLOW"dir %b\n" NA, dir);
	return (dir != 0);
}

bool		Chunk::conditionValidate(vector<vbo_type> &tempVbo, BlockPos posInMesh, bool &b)
{
	if (this->getBlock(posInMesh).getInfo().id == AIR
		|| !this->canPrintBlock(tempVbo, posInMesh))
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float), (void*)(sizeof(float) * 3));
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

void		Chunk::updateFenced(int source)
{
	if (source)
	{
		Chunk* tmp;
		if ((tmp = this->getNeighboor(NORTH)))
			tmp->updateFenced(0);
		if ((tmp = this->getNeighboor(SOUTH)))
			tmp->updateFenced(0);
		if ((tmp = this->getNeighboor(EAST)))
			tmp->updateFenced(0);
		if ((tmp = this->getNeighboor(WEST)))
			tmp->updateFenced(0);
	}
	if (this->getNeighboor(NORTH) && this->getNeighboor(SOUTH) && this->getNeighboor(EAST) && this->getNeighboor(WEST))
		this->state = FENCED;
	else
		this->state = UNFENCED;
}

Chunk		*Chunk::getNeighboor(Direction dir)
{
	if (g_dir_c[dir].axis == Y || g_dir_c[dir].axis == -Y)
		return NULL;
	return this->world->get(this->pos + g_dir_c[dir].chunk_vec);
}

Block		*Chunk::getBlockNeighboor(BlockPos pos, Direction dir) // Fonction peut etre opti
{
	struct s_direction_consts&	c(g_dir_c[dir]);
	Chunk						*neighboor;
	short						axis;

	axis = c.axis < 0 ? -c.axis : c.axis;
	if (pos[axis] == 15 && c.block_vec[axis] == 1)
	{
		pos[axis] = 0;
		if (axis == Y && pos[MY] < 15)
		{
			neighboor = this;
			pos[MY] = pos[MY] + 1;
		}
		else
			neighboor = this->getNeighboor(dir);
		if (neighboor)
			return &neighboor->getBlock(pos);
		return NULL;
	}
	else if (pos[axis] == 0 && c.block_vec[axis] == -1)
	{
		pos[axis] = 15;
		if (axis == Y && pos[MY] > 0)
		{
			neighboor = this;
			pos[MY] = pos[MY] - 1;
		}
		else
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

	t = e.getTexture(0);
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
			"nbTxt"), END_BLOCK_T);
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