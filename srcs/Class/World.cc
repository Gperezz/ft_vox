/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2021/11/11 16:47:33 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"
#include <iostream>
using namespace std;

World::World(Engine& engine, unsigned long *seed)
:
	queueOn(true),
	enginePtr(engine),
	deltaFrameTime(0.0),
	lastFrameTime(0.0)
{
	this->initQueueSorter();
	// this->initQueueThread();
	this->worldGen.configure(seed);
}

World::World(Engine& engine, string& path, unsigned long *seed)
:
	queueOn(true),
	enginePtr(engine),
	path(path),
	deltaFrameTime(0.0),
	lastFrameTime(0.0)
{
	this->initQueueSorter();
	// this->initQueueThread();
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
	this->queueOn = false;
	// queueThread.join();
}

void	World::initQueueSorter()
{
	auto cmp = [this](ChunkPos a, ChunkPos b)->bool{
		ChunkPos center = this->getCameraChunkPos();
		// printf("center : %d, %d\n", center[0], center[1]);
		float da = a.distance(center);
		float db = b.distance(center);
		if (da == db)
			return a < b;
		return da < db;
	};
	this->loadQueue = set<ChunkPos, function<bool (ChunkPos, ChunkPos)>>(cmp);
}

bool	World::LoadNextQueuedChunk()
{
	size_t size;
	{
		unique_lock<mutex> lock(this->queueMutex);
		size = this->loadQueue.size(); 
	}
	if (size)
	{
		decltype(this->loadQueue.begin()) pos;
		{
			unique_lock<mutex> lock(this->queueMutex);
			pos = this->loadQueue.begin();
			this->loadQueue.erase(pos);
		}
		this->loadChunk(*pos);
		return true;
	}
	return false;
}

// void	World::initQueueThread()
// {
// 	auto queueLoop = [this]() {
// 		while (this->queueOn){
// 			this->LoadNextQueuedChunk();
// 		}
// 	};
// 	this->queueThread = thread(queueLoop);
// }

void	World::deleteFar()
{
	static ChunkPos	prevPos;
	ChunkPos		pos = this->getCameraChunkPos();
	ChunkPos		chunkP;

	if (pos == prevPos)
		return ;
	unique_lock<mutex> lockMem(this->memoryMutex);
	for (auto it = this->memoryChunks.begin(); it != this->memoryChunks.end(); it++)
	{
		chunkP = it->first;
		if (chunkP.get(0) < pos.get(0) - CHK_DEL_DIST_MEM
			|| chunkP.get(0) > pos.get(0) + CHK_DEL_DIST_MEM
			|| chunkP.get(1) < pos.get(1) - CHK_DEL_DIST_MEM
			|| chunkP.get(1) > pos.get(1) + CHK_DEL_DIST_MEM)
		{
			Chunk* delChunk = it->second;
			std::cout << RED << "Chunk " << chunkP.get(0) << " " << chunkP.get(1) << "\n";
			std::cout << GREEN << this->memoryChunks.count(chunkP) << "\n";

			delete delChunk;
			this->memoryChunks[chunkP] = NULL;
			// this->memoryChunks[chunkP]->updateFenced(1); // mettre un delete updateFenced
			this->memoryChunks.erase(chunkP);
		}
	}
	pos = prevPos;
}

void	World::deleteFarInDisplay()
{
	static ChunkPos	prevPos;
	ChunkPos		pos = this->getCameraChunkPos();
	ChunkPos		chunkP;

	if (pos == prevPos)
		return ;

	{unique_lock<mutex> lockQueue(this->queueMutex);
	for (auto it = this->loadQueue.begin(); it != this->loadQueue.end(); it++)
	{
		chunkP = *it;
		if (chunkP.get(0) < pos.get(0) - CHK_DEL_DIST
			|| chunkP.get(0) > pos.get(0) + CHK_DEL_DIST
			|| chunkP.get(1) < pos.get(1) - CHK_DEL_DIST
			|| chunkP.get(1) > pos.get(1) + CHK_DEL_DIST)
				this->loadQueue.erase(chunkP);
	}}

	{unique_lock<mutex> lock(this->displayedMutex);
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
	{
		chunkP = *it;
		if (chunkP.get(0) < pos.get(0) - CHK_DEL_DIST
			|| chunkP.get(0) > pos.get(0) + CHK_DEL_DIST
			|| chunkP.get(1) < pos.get(1) - CHK_DEL_DIST
			|| chunkP.get(1) > pos.get(1) + CHK_DEL_DIST)
				this->displayedChunks.erase(it);
	}}
	pos = prevPos;
}


void	World::rearrangeQueues()
{
	static ChunkPos prevPos;
	static bool	start = true;
	ChunkPos pos = this->getCameraChunkPos();

	if (prevPos == pos && !start)
		return ;
	start = false;
	unique_lock<mutex> lock(this->queueMutex);
	for (int i = -CHK_RND_DIST; i < CHK_RND_DIST + 1; i++)
	{
		for (int j = -CHK_RND_DIST; j < CHK_RND_DIST + 1; j++)
		{
			ChunkPos cp(pos + (int[]){i, j});
			if (this->loadQueue.count(cp) == 0)
				this->loadQueue.insert(cp);
		}
	}
	prevPos = pos;
}

void	World::display(Engine &e, float currentFrameTime)
{
	this->rearrangeQueues();
	this->deleteFarInDisplay();
	this->deleteFar();

	this->LoadNextQueuedChunk();
	if (e.isSkybox() && e.getTexture(1))
		e.displaySky(e.getTexture(1));
	unique_lock<mutex> lk(this->displayedMutex);
	// unique_lock<mutex> lk2(this->deltaFTMutex);
	unique_lock<mutex> lk3(this->memoryMutex);
	
	Textures	*texture = e.getTexture(0);
	Shader		&shader = e.getShader();
	Camera		cam = e.getCam();

	std::map<ChunkPos, Chunk*>::iterator	chunk;
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
	{
		chunk = this->memoryChunks.find(*it);
		if (chunk != this->memoryChunks.end())
			chunk->second->displayChunk(cam, shader, texture);
	}
	this->deltaFrameTime = currentFrameTime - this->lastFrameTime;
	this->lastFrameTime = currentFrameTime;
}

ChunkPos	World::getCameraChunkPos()
{
	glm::vec3 mat;
	mat = this->enginePtr.getCam().getTranslate();
	float cam[] = {mat.x / 16, mat.z / 16};
	if (cam[0] < 0)
		cam[0] -= 1.0;
	if (cam[1] < 0)
		cam[1] -= 1.0;
	return (int[]){(int)cam[0], (int)cam[1]};
}

Chunk	*World::get(ChunkPos cp)
{
	if (this->memoryChunks.count(cp) == 0)
		return NULL;
	return this->memoryChunks.at(cp);
}

Chunk	*World::operator[](ChunkPos cp)
{
	return this->get(cp);
}

void	World::pushInDisplay(Chunk* chunk, bool alreadyLoad)
{
	Chunk*	tmp;
	ChunkPos chunkP;

	pair<unordered_set<ChunkPos>::iterator, bool> ret(this->displayedChunks.begin(), false);

	if (!chunk)
		return;
	chunkP = chunk->getPos();

	if (chunk->getFenced())
	{
		ret = this->displayedChunks.insert(chunk->getPos());
		if (ret.second && !alreadyLoad)
			chunk->generateGraphics();
	}
	int		i = 0;
	while (i < 4 && !alreadyLoad)
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
	{unique_lock<mutex> lockDisp(this->displayedMutex);
		if (this->displayedChunks.count(cp))
			return;
	} // Check s'il est deja afficher

	Chunk	*newChunk = NULL;
	{unique_lock<mutex> lockMem(this->memoryMutex); // On check tout au long de la fonction si le chunk existe encore dans memory
		std::map<ChunkPos, Chunk*>::iterator findChunk = this->memoryChunks.find(cp);
		if (findChunk != this->memoryChunks.end())
		{
			this->memoryChunks[cp]->updateFenced(1);
			newChunk = findChunk->second;
			{unique_lock<mutex> lockDisp(this->displayedMutex);
				this->pushInDisplay(newChunk, true);
			}
			return;
		}
	}
	newChunk = new Chunk(this, cp);
	this->worldGen.genChunk(newChunk);
	
	{unique_lock<mutex> lockMem(this->memoryMutex);
	this->memoryChunks.insert(std::pair<ChunkPos, Chunk*>(cp, newChunk));
	this->memoryChunks[cp]->updateFenced(1);}
	
	{unique_lock<mutex> lockDisp(this->displayedMutex);
		this->pushInDisplay(newChunk, false);}

}

map<ChunkPos, Chunk*>	&World::getMapMemory(void)
{
	return (this->memoryChunks);
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

float	World::getDeltaFrameTime(void)
{
	// unique_lock<mutex> lk(this->deltaFTMutex);
	return (this->deltaFrameTime);
}
