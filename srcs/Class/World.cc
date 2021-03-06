/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/10/19 19:56:21 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"
#include <iostream>

using namespace std;

World::World(unsigned long *seed)
{
	this->worldGen.configure(seed);
	this->deltaFrameTime = 0.0;
	this->lastFrameTime = 0.0;
}

World::World(string& path, unsigned long *seed)
{
	this->path = path;
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

void	World::display(Engine &e, float currentFrameTime)
{
	if (e.isSkybox() && e.getTexture(SKY_T - END_BLOCK_T))
		e.displaySky(e.getTexture(SKY_T - END_BLOCK_T));
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
		this->memoryChunks.at(*it)->displayChunk(e);
	this->deltaFrameTime = currentFrameTime - this->lastFrameTime;
	this->lastFrameTime = currentFrameTime;
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

void	World::pushInDisplay(Chunk* chunk)
{
	Chunk*	tmp;
	pair<unordered_set<ChunkPos>::iterator, bool> ret(this->getDisplayedChunks().begin(), false);
	int		i = 0;

	if (chunk->getFenced())
	{
		ret = this->displayedChunks.insert(chunk->getPos()); // displayQueue
		if (ret.second)
			chunk->generateGraphics();
	}
	while (i < 4)
	{
		tmp = chunk->getNeighboor((Direction)i);
		if (tmp && tmp->getFenced())
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
		this->worldGen.genTest(newChunk);
		this->memoryChunks[cp] = newChunk;
		this->memoryChunks[cp]->updateFenced(1);
		this->pushInDisplay(newChunk);
	}
}

Chunk	*World::getMemoryChunk(ChunkPos pos)
{
	return (this->memoryChunks.at(pos));
}

unordered_set<ChunkPos>	&World::getDisplayedChunks(void)
{
	return (this->displayedChunks);
}

void	World::loadChunk(int x, int z)
{
	return (this->loadChunk(ChunkPos((int[2]){x, z})));
}

float	World::getDeltaFrameTime(void) const
{
	return (this->deltaFrameTime);
}