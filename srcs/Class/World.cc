/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/07/29 23:23:13 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"
#include <iostream>

using namespace std;
using namespace std::filesystem;

World::World(unsigned long *seed)
{
	this->worldGen.configure(seed);
}

World::World(string& pathStr, unsigned long *seed)
{
	this->rootDirPath.assign(pathStr);
	this->worldGen.configure(seed);
}

World::World(path& p, unsigned long *seed)
{
	this->rootDirPath.assign(p);
	this->worldGen.configure(seed);
}

World::~World()
{
	std::map<ChunkPos, Chunk*>::iterator it = memoryChunks.begin();

	while (it != memoryChunks.end())
	{
		delete it->second;
		it++;
	}
}

void	World::display(Engine &e)
{
	size_t	i;

	i = 0;
	while (i < this->displayedChunks.size())
	{
		// ft_printf(BLUE "Display Chunk{%d %d}\n" NA, this->displayedChunks[i].get(0),
		// 	this->displayedChunks[i].get(1));
		this->memoryChunks.at(this->displayedChunks[i])->displayChunk(e, this->getWorldMat().getMatrix(true));
		i++;
	}
	// ft_printf("\n");
}

path	World::getDir(){
	return this->rootDirPath;
}

Chunk	*World::get(ChunkPos cp)
{
	if (this->memoryChunks.count(cp) == 0)
		return NULL;
	return this->memoryChunks.at(cp); // Potentiellement optimisable
}

Chunk	*World::operator[](ChunkPos cp)
{
	return this->get(cp);
}

void	World::pushInDisplay(ChunkPos cp)
{
	Chunk*	base = this->memoryChunks[cp];
	Chunk*	tmp;
	pair<set<ChunkPos>::iterator, bool> ret;
	int		i = 0;
	if (base->getFenced())
	{
		ret = this->displayedChunks.insert(base->getPos()); // displayQueue
		if (ret.second)
			base->generateGraphics();
	}
	while (i < 4)
	{
		tmp = base->getNeighboor(i);
		if (tmp && tmp->getFenced)
		{
			ret = this->displayedChunks.insert(tmp->getPos());
			if (ret.second)
				tmp->generateGraphics();
		}
		i++;
	}
}

void	World::loadChunk(ChunkPos cp)
{
	printf(YELLOW "%d %d\n" NA, cp.get(0), cp.get(1));
	if (this->memoryChunks.count(cp) == 0)
	{
		Chunk	*newChunk = new Chunk(this, cp);
		this->worldGen.genChunk(newChunk);
		this->memoryChunks[cp] = newChunk;
		this->memoryChunks[cp]->updateFenced(1);
		this->pushInDisplay(cp);
	}
}

Chunk	*World::getMemoryChunk(ChunkPos pos)
{
	return (this->memoryChunks.at(pos));
}

vector<ChunkPos>	&World::getDisplayedChunks(void)
{
	return (this->displayedChunks);
}

void	World::loadChunk(int x, int z)
{
	return this->loadChunk(ChunkPos((int[2]){x, z}));
}

Mat		&World::getWorldMat(void)
{
	return (this->worldMatrix);
}
