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

void	World::loadChunk(ChunkPos cp)
{
	printf(YELLOW "%d %d\n" NA, cp.get(0), cp.get(1));
	if (this->memoryChunks.count(cp) == 0)
	{
		Chunk	*newChunk = new Chunk(this, cp);
		this->worldGen.genChunk(newChunk);
		this->memoryChunks[cp] = newChunk;
		// this->memoryChunks[cp]->printSlice(0);
		this->memoryChunks[cp]->updateFenced();
		if (this->memoryChunks[cp]->getFenced())
		{
			this->memoryChunks[cp]->generateGraphics();
			this->displayedChunks.push_back(this->memoryChunks[cp]->getPos()); // displayQueue
		}
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
