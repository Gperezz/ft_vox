/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/10/26 00:23:05 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"
#include <iostream>
using namespace std;

World::World(unsigned long *seed)
{
	this->queueOn = true;
	this->initQueueSorter();
	this->initQueueThread();
	this->worldGen.configure(seed);
}

World::World(string& path, unsigned long *seed)
{
	this->queueOn = true;
	this->initQueueSorter();
	this->initQueueThread();
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
	
	queueThread.join();
}

void	World::initQueueSorter()
{
	auto cmp = [this](ChunkPos a, ChunkPos b)->bool{
		ChunkPos center = this->getCameraChunkPos();
		float da = a.distance(center);
		float db = b.distance(center);
		return da < db;
	};
	this->loadQueue = set<ChunkPos, function<bool (ChunkPos, ChunkPos)>>(cmp);
}

void	World::initQueueThread()
{
	auto queueLoop = [this]() {
		while (this->queueOn){
			if (this->loadQueue.size()){
				printf("something's up");
				decltype(this->loadQueue.begin()) pos;
				{
					unique_lock<mutex> lock(this->queueMutex);
					pos = this->loadQueue.begin();
					this->loadQueue.erase(pos);
				}
				this->loadChunk(*pos);
			}
		}
	};
	this->queueThread = thread(queueLoop);
}

void	World::display(Engine &e)
{
	this->rearrangeQueues();
	if (e.isSkybox() && e.getTexture(SKY_T - END_BLOCK_T))
		e.displaySky(e.getTexture(SKY_T - END_BLOCK_T));
	{
		unique_lock<mutex> lock(this->queueMutex);
		for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
			this->memoryChunks.at(*it)->displayChunk(e, this->getWorldMat().getMatrix(true));
	}
}

void	World::rearrangeQueues()
{
	static ChunkPos pos;
	ChunkPos newPos = this->getCameraChunkPos();
	printf("%d %d\n", pos[0], pos[1]);

	if (pos == newPos)
		return ;
	pos = newPos;
	for (int i = -CHK_RND_DIST; i < CHK_RND_DIST; i++) {
		for (int j = -CHK_RND_DIST; j < CHK_RND_DIST; j++){
			{
				unique_lock<mutex> lock(this->queueMutex);
				this->loadQueue.insert(pos + (int[]){i, j});
			}
		}
	}
}

ChunkPos	World::getCameraChunkPos()
{
	glm::vec3 mat = this->getWorldMat().getTranslate();
	float test[] = {-mat.x / 16, -mat.z / 16};
	if (test[0] < 0)
		test[0] -= 1.0;
	if (test[1] < 0)
		test[1] -= 1.0;
	return (int[]){(int)test[0], (int)test[1]};
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
		this->worldGen.genChunk(newChunk);
		{
			unique_lock<mutex> lock(this->queueMutex);
			this->memoryChunks[cp] = newChunk;
		}
		this->memoryChunks[cp]->updateFenced(1);
		{
			unique_lock<mutex> lock(this->queueMutex);
			this->pushInDisplay(newChunk);
		}
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
	return this->loadChunk(ChunkPos((int[2]){x, z}));
}

Mat		&World::getWorldMat(void)
{
	return (this->worldMatrix);
}
