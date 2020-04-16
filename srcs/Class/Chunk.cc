/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2020/04/16 07:43:50 by gperez           ###   ########.fr       */
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

bool		Chunk::blockSurrounded(vector<vbo_type> &tempVbo, BlockPos posMesh)
{
	char	dir;
	int		i;

	i = NORTH;
	dir = 0;
	while (i < 6)
	{
		if (Chunk::getBlockNeighboor(posMesh, (Direction)i)->getInfo().id != 0)
		{
			dir += 1;
			fillTempVbo(tempVbo, (BlockPos*)g_dir_c[i].pts, posMesh,
				this->get(posMesh).getInfo().id);
		}
		i++;
	}
	return (dir == 6);
}

bool		Chunk::conditionValidate(vector<vbo_type> &tempVbo, BlockPos posMesh, bool &b)
{
	if (this->get(posMesh).getInfo().id == AIR
		|| this->blockSurrounded(tempVbo, posMesh))
		return (0);
	b = 1;
}

void		Chunk::generateGraphics(void)
{
	unsigned int	i;

	i = 0;
	if (Chunk::state == UNFENCED)
		return ;
	Chunk::validateChunk();
}

void		Chunk::generateVbo(char index, vector<vbo_type> tempVbo)
{
	glGenVertexArrays(1, &(Chunk::tabVao[index]));
	glBindVertexArray(tabVao[index]);
	glGenBuffers(1, &(Chunk::tabVbo[index]));
	glBindBuffer(GL_ARRAY_BUFFER, tabVbo[index]);
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
	glDeleteBuffers(1, &(Chunk::tabVbo[index]));
	glDeleteVertexArrays(1, &(Chunk::tabVao[index]));
}

void		Chunk::validateMesh(char meshIdx)
{
	BlockPos				pos; // [meshY][x][y][z]
	bool					validateValue;
	vector<vbo_type>		tempVbo;

	if (Chunk::valid.find(meshIdx) != Chunk::valid.end())
		deleteVbo(meshIdx);
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
		this->valid.insert(meshIdx, tempVbo.size() * 6);
	}
}

void		Chunk::validateChunk(void)
{
	for (unsigned i = 0; i < 16; i++)
		validateMesh(i);
}

void		Chunk::displayChunk(unsigned int prog)
{
	std::map<char, unsigned int>::iterator it = Chunk::valid.begin();

	while (it != Chunk::valid.end())
	{
		glBindVertexArray(Chunk::tabVao[it->first]);
		glUseProgram(prog);
		// glUniform1i(glGetUniformLocation(prog, "basicTexture"), i_t >= T_END_OBJ
			// ? g_objs_entities[0].index_txt : g_objs_entities[entities[i]->getType()].index_txt);
		// glUniform2f(glGetUniformLocation(prog, "envx"),
			// entities[i]->getXMin(), entities[i]->getXMax());
		// glUniform2f(glGetUniformLocation(prog, "envy"),
			// entities[i]->getYMin(), entities[i]->getYMax());
		// glUniform2f(glGetUniformLocation(prog, "envz"),
			// entities[i]->getZMin(), entities[i]->getZMax());
		// glUniformMatrix4fv(glGetUniformLocation(prog,
		// 	"model"), 1, GL_FALSE, glm::value_ptr(entities[i]->calcMatrix()));
		// glUniformMatrix4fv(glGetUniformLocation(prog,
		// 	"view"), 1, GL_FALSE, glm::value_ptr(cam.calcMatrix()));
		// glUniformMatrix4fv(glGetUniformLocation(prog,
		// 	"projection"), 1, GL_FALSE, glm::value_ptr(cam.getProjMatrix()));
		glDrawArrays(GL_TRIANGLES, 0, it->second);
		it++;
	}
}

Chunk		*Chunk::getNeighboor(Direction dir)
{
	if (g_dir_c[dir].axis == Y)
		return this;
	return this->world->get(this->pos + g_dic_c[dir].chunk_vec);
}

Block		*Chunk::getBlockNeighboor(BlockPos pos, Direction dir)
{
	struct s_direction_consts&	c;
	int							limit;
	int							rev_limit;

	c = g_dir_c[dir];
	if (pos[c.axis] == limit)
	{
		if (c.axis == Y)
		{
			if (pos[MY] == c.sens ? 15 : 0)
				return NULL;
			pos[MY] += c.sens ? 1 : -1;
			pos[Y] = c.sens ? 0 : 15;
			return &this->get(pos);
		}
		else
		{
			pos[c.axis] = c.sens ? 0 : 15;
			return &this->getNeighboor(dir)->get(pos);
		}
	}
	return &this->get(pos + c.block_vec);
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