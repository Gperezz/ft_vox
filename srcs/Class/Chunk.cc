/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 16:00:52 by gperez            #+#    #+#             */
/*   Updated: 2021/12/01 23:46:47 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Chunk.hpp"
#include "World.hpp"

using namespace std;

Chunk::Chunk()
{
	this->state = UNFENCED;
	bzero(this->blocks, sizeof(this->blocks));
	this->generate = false;
	bzero(this->tabVao, sizeof(unsigned int) * 16);
	bzero(this->tabVbo, sizeof(unsigned int) * 16);
	bzero(this->tabVaoWater, sizeof(unsigned int) * 16);
	bzero(this->tabVboWater, sizeof(unsigned int) * 16);
}

Chunk::Chunk(World *w)
{
	this->state = UNFENCED,
	this->world = w;
	bzero(this->blocks, sizeof(this->blocks));
	this->generate = false;
	bzero(this->tabVao, sizeof(unsigned int) * 16);
	bzero(this->tabVbo, sizeof(unsigned int) * 16);
	bzero(this->tabVaoWater, sizeof(unsigned int) * 16);
	bzero(this->tabVboWater, sizeof(unsigned int) * 16);
}

Chunk::Chunk(World *w, ChunkPos pos)
{
	this->state = UNFENCED,
	this->pos = pos;
	this->world = w;
	bzero(this->blocks, sizeof(this->blocks));
	this->generate = false;
	bzero(this->tabVao, sizeof(unsigned int) * 16);
	bzero(this->tabVbo, sizeof(unsigned int) * 16);
	bzero(this->tabVaoWater, sizeof(unsigned int) * 16);
	bzero(this->tabVboWater, sizeof(unsigned int) * 16);
}

Chunk::Chunk(const Chunk& copy)
{
	this->operator=(copy);
}

Chunk::~Chunk()
{
	this->deleteVbos();
}


void	Chunk::fillTempVbo(vector<vbo_type> &tempVbo, t_direction_consts dir_c, BlockPos posInMesh, t_id	id)
{
	int			iPt;
	vbo_type	vboType;
	short		idTxtBitwise;
	short		idBitwise = 0;

	iPt = 0;
	while (iPt < 6)
	{
		vboType.pos[0] = dir_c.pts[iPt].get(X) + posInMesh.get(X) + this->getPos().get(0) * 16;
		vboType.pos[1] = dir_c.pts[iPt].get(Y) + posInMesh.get(Y) + posInMesh.get(MY) * 16;
		vboType.pos[2] = dir_c.pts[iPt].get(Z) + posInMesh.get(Z) + this->getPos().get(1) * 16;

		vboType.normal[0] = dir_c.pts[iPt].get(X) - (float)LENGTH_BLOCK / 2.;
		vboType.normal[1] = dir_c.pts[iPt].get(Y) - (float)LENGTH_BLOCK / 2.;
		vboType.normal[2] = dir_c.pts[iPt].get(Z) - (float)LENGTH_BLOCK / 2.;
	
		vboType.meta = dir_c.axis < 0 ? dir_c.axis + 7 : dir_c.axis;

		if (vboType.meta == 1 || vboType.meta == 6)
		{
			vboType.coords[0] = dir_c.pts[iPt].get(Z);
			vboType.coords[1] = dir_c.pts[iPt].get(Y);
		}
		else if (vboType.meta == 2 || vboType.meta == 5)
		{
			vboType.coords[0] = dir_c.pts[iPt].get(X);
			vboType.coords[1] = dir_c.pts[iPt].get(Z);
		}
		else
		{
			vboType.coords[0] = dir_c.pts[iPt].get(X);
			vboType.coords[1] = dir_c.pts[iPt].get(Y);
		}
		idTxtBitwise = id.txt << 3;
		idBitwise = id.type << 8;
		vboType.meta = vboType.meta | idTxtBitwise | idBitwise;
		tempVbo.push_back(vboType);
		iPt++;
	}
}
void		Chunk::canPrintBlockLoop(vector<vbo_type> &tempVbo, vector<vbo_type> &tempVboTrans, BlockPos posInMesh, int &i, char &dir, bool &isTrans)
{
	Block *tmp = this->getBlockNeighboor(posInMesh, (Direction)i);
	if ((!tmp || Block::isTransparentBlock(*tmp)))
	{
		e_BlockType type = (e_BlockType)this->getBlock(posInMesh).getInfo().id;
		if (type == WATER && tmp->getInfo().id == AIR)
		{
			this->fillTempVbo(tempVboTrans, (t_direction_consts)g_dir_c[i], posInMesh, (t_id){
			Textures::getIndexTxt(type), type});
			isTrans = true;
		}
		else if (type == WATER)
			return;
		else
		{
			this->fillTempVbo(tempVbo, (t_direction_consts)g_dir_c[i], posInMesh, (t_id){
				Textures::getIndexTxt(type), type});
		}
		dir += 1 << i; //Faces visibles
	}
}

bool		Chunk::canPrintBlock(std::vector<vbo_type> &tempVbo, std::vector<vbo_type> &tempVboTrans, BlockPos posInMesh, bool &isTrans)
{
	char	dir = 0;
	int		i = NORTH;

	while (i < 6)
	{
		canPrintBlockLoop(tempVbo, tempVboTrans, posInMesh, i, dir, isTrans);
		i++;
	}
	return (dir != 0);
}

void		Chunk::conditionValidate(std::vector<vbo_type> &tempVbo, vector<vbo_type>	&tempVboTrans, BlockPos posInMesh, bool &b, bool &bT)
{
	bool	isTrans = false;
	if (this->getBlock(posInMesh).getInfo().id == AIR
		|| !this->canPrintBlock(tempVbo, tempVboTrans, posInMesh, isTrans))
		return;
	if (isTrans)
		bT = 1;
	else
		b = 1;
}

void		Chunk::generateVbo(char index, vector<vbo_type> tempVbo, bool isT)
{
	if (isT)
	{
		glBindVertexArray(tabVaoWater[(int)index]);
		glBindBuffer(GL_ARRAY_BUFFER, tabVboWater[(int)index]);
	}
	else
	{
		glBindVertexArray(tabVao[(int)index]);
		glBindBuffer(GL_ARRAY_BUFFER, tabVbo[(int)index]);
	}
	glBufferData(GL_ARRAY_BUFFER, tempVbo.size() * sizeof(vbo_type), &tempVbo[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)(sizeof(float) * 6));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8 + sizeof(float), (void*)(sizeof(float) * 8));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

}

void		Chunk::validateMesh(char meshIdx)
{
	BlockPos				pos; // [meshY][x][y][z]
	bool					validateValue = 0;
	bool					validateValueTrans = 0;
	vector<vbo_type>		tempVbo;
	vector<vbo_type>		tempVboTrans;

	if (this->valid.find(meshIdx) != this->valid.end())
		this->valid.erase(meshIdx);
	if (this->validTrans.find(meshIdx) != this->validTrans.end())
		this->validTrans.erase(meshIdx);
	pos[MY] = meshIdx;
	while (pos[X] < 16)
	{
		while (pos[Y] < 16)
		{
			while (pos[Z] < 16)
			{
				conditionValidate(tempVbo, tempVboTrans, pos, validateValue, validateValueTrans);
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
		generateVbo(meshIdx, tempVbo, false);
		this->valid.insert({meshIdx, tempVbo.size()});
	}
	if (validateValueTrans && tempVboTrans.size())
	{
		generateVbo(meshIdx, tempVboTrans, true);
		this->validTrans.insert({meshIdx, tempVboTrans.size()});
	}
}

void		Chunk::deleteVbos(void)
{
	if (this->generate)
	{
		glDeleteBuffers(16, &(this->tabVbo[0]));
		glDeleteVertexArrays(16, &(this->tabVao[0]));
		glDeleteBuffers(16, &(this->tabVboWater[0]));
		glDeleteVertexArrays(16, &(this->tabVaoWater[0]));
		this->generate = false;
	}
}

////////////////////////////// Public //////////////////////////////

bool		Chunk::isGenerated(void)
{
	return (this->generate);
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

Block&		Chunk::getBlock(BlockPos pos)
{
	return (this->getBlock(pos[MY], pos[X], pos[Y], pos[Z]));
}

Block&		Chunk::getBlock(int my, int x, int y, int z)
{
	return (this->blocks[my][x][y][z]);
}

void		Chunk::setBlock(BlockPos blockPos, t_block_info info)
{
	this->blocks[blockPos.get(0)][blockPos.get(1)][blockPos.get(2)][blockPos.get(3)] = info;
}

void		Chunk::setBlock(BlockPos blockPos, t_block_info info, unsigned char biome)
{
	this->blocks[blockPos.get(0)][blockPos.get(1)][blockPos.get(2)][blockPos.get(3)] = info;
	// printBiome(biome);
	this->blocks[blockPos.get(0)][blockPos.get(1)][blockPos.get(2)][blockPos.get(3)].setBiome(biome);
}

unsigned char Chunk::getBiome(BlockPos blockPos)
{
	return (this->blocks[blockPos.get(0)][blockPos.get(1)][blockPos.get(2)][blockPos.get(3)].getBiome());
}

ChunkPos	Chunk::getPos(void)
{
	return (this->pos);
}

bool	Chunk::getFenced(void)
{
	return (this->state);
}

void	Chunk::setUnfenced(void)
{
	this->state = UNFENCED;
}

void		Chunk::updateDelFenced(void)
{
	Chunk* tmp;
	if ((tmp = this->getNeighboorUnsafe(NORTH)))
		tmp->setUnfenced();
	if ((tmp = this->getNeighboorUnsafe(SOUTH)))
		tmp->setUnfenced();
	if ((tmp = this->getNeighboorUnsafe(EAST)))
		tmp->setUnfenced();
	if ((tmp = this->getNeighboorUnsafe(WEST)))
		tmp->setUnfenced();
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

void		Chunk::updateFencedUnsafe(int source)
{
	if (source)
	{
		Chunk* tmp;
		if ((tmp = this->getNeighboorUnsafe(NORTH)))
			tmp->updateFencedUnsafe(0);
		if ((tmp = this->getNeighboorUnsafe(SOUTH)))
			tmp->updateFencedUnsafe(0);
		if ((tmp = this->getNeighboorUnsafe(EAST)))
			tmp->updateFencedUnsafe(0);
		if ((tmp = this->getNeighboorUnsafe(WEST)))
			tmp->updateFencedUnsafe(0);
	}
	if (this->getNeighboorUnsafe(NORTH) && this->getNeighboorUnsafe(SOUTH) && this->getNeighboorUnsafe(EAST) && this->getNeighboorUnsafe(WEST))
		this->state = FENCED;
	else
		this->state = UNFENCED;
}

Chunk		*Chunk::getNeighboorUnsafe(Direction dir)
{
	if (g_dir_c[dir].axis == Y || g_dir_c[dir].axis == -Y)
		return NULL;
	if (this->world)
		return this->world->getUnsafe(this->pos + g_dir_c[dir].chunk_vec);
	return NULL;
}

Chunk		*Chunk::getNeighboor(Direction dir)
{
	if (g_dir_c[dir].axis == Y || g_dir_c[dir].axis == -Y)
		return NULL;
	if (this->world)
		return this->world->get(this->pos + g_dir_c[dir].chunk_vec);
	return NULL;
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
			neighboor = this->getNeighboorUnsafe(dir);
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
			neighboor = this->getNeighboorUnsafe(dir);
		if (neighboor)
			return &neighboor->getBlock(pos);
		return NULL;
	}
	return &this->getBlock(pos + c.block_vec);
}

void		Chunk::generateGraphics(unsigned int mesh)
{
	if (this->state == FENCED && mesh < 16)
		validateMesh(mesh);
	this->generate = true;
}

void		Chunk::generateGraphics(void)
{
	if (!this->generate)
	{
		glGenVertexArrays(16,  &(this->tabVao[0]));
		glGenBuffers(16,  &(this->tabVbo[0]));
		glGenVertexArrays(16,  &(this->tabVaoWater[0]));
		glGenBuffers(16,  &(this->tabVboWater[0]));
		this->generate = true;
	}
	for (int i = 15; i >= 0; i--)
		validateMesh((unsigned int)i);
}

void		Chunk::displayChunk(Camera cam, Shader shader, Textures *t)
{
	std::map<char, unsigned int>::iterator	it = this->valid.begin();

	while (it != this->valid.end())
	{
		glBindVertexArray(this->tabVao[(int)it->first]);
		glUseProgram(shader.getProgram());
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"view"), 1, GL_FALSE, glm::value_ptr(cam.getMatrix(false)));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"projection"), 1, GL_FALSE, glm::value_ptr(cam.getProjMatrix()));
		glUniform1i(glGetUniformLocation(shader.getProgram(),
			"nbTxt"), END_BLOCK_T);
		glUniform1i(glGetUniformLocation(shader.getProgram(),
			"basicTexture"), t ? t->getTxt() : 0);
		glDrawArrays(GL_TRIANGLES, 0, it->second);
		it++;
	}
}

void		Chunk::displayChunkTransparency(Camera cam, Shader shader, Textures *t)
{
	std::map<char, unsigned int>::iterator	it = this->validTrans.begin();

	while (it != this->validTrans.end())
	{
		glBindVertexArray(this->tabVaoWater[(int)it->first]);
		glUseProgram(shader.getProgram());
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"view"), 1, GL_FALSE, glm::value_ptr(cam.getMatrix(false)));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(),
			"projection"), 1, GL_FALSE, glm::value_ptr(cam.getProjMatrix()));
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