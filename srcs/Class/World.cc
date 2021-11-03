/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2021/10/13 20:00:46 by gperez           ###   ########.fr       */
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

bool	World::LoadNextQueuedChunk(){
	size_t size;
	{	unique_lock<mutex> lock(this->queueMutex);
		size = this->loadQueue.size(); 
	}
	if (size){
		decltype(this->loadQueue.begin()) pos;
		{	unique_lock<mutex> lock(this->queueMutex);
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

void	World::display(Engine &e, float currentFrameTime)
{
	this->rearrangeQueues();

	this->LoadNextQueuedChunk();
	if (e.isSkybox() && e.getTexture(1))
		e.displaySky(e.getTexture(1));
	unique_lock<mutex> lk(this->displayedMutex);
	unique_lock<mutex> lk2(this->deltaFTMutex);
	unique_lock<mutex> lk3(this->memoryMutex);
	
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
		this->memoryChunks.at(*it)->displayChunk(e.getCam(), e.getShader(), e.getTexture(0));
	this->deltaFrameTime = currentFrameTime - this->lastFrameTime;
	this->lastFrameTime = currentFrameTime;
}

void	World::rearrangeQueues()
{
	static ChunkPos pos;
	static bool	start = true;
	ChunkPos newPos = this->getCameraChunkPos();
	// printf("cur pos: %d %d\n", newPos[0], newPos[1]);

	if (pos == newPos && !start)
		return ;
	start = false;
	pos = newPos;

// TEST
	// unique_lock<mutex>	lockMem(this->memoryMutex);
	// ChunkPos			compare;

	// for (map<ChunkPos, Chunk*>::iterator itMem = memoryChunks.begin(); itMem != memoryChunks.end(); itMem++)
	// {
	// 	compare = itMem->first;
	// 	if (pos.distance(compare) > CHK_DEL_DIST)
	// 	{
	// 		unique_lock<mutex>	lockDisplay(this->displayedMutex);
	// 		if (this->displayedChunks.find(compare) != this->displayedChunks.end())
	// 			this->displayedChunks.erase(compare);
	// 		unique_lock<mutex>	lockQueue(this->queueMutex);
	// 		if (this->loadQueue.find(compare) != this->loadQueue.end())
	// 			this->loadQueue.erase(compare);
	// 		delete itMem->second;
	// 		this->memoryChunks.erase(itMem);
	// 	}
	// }
// TEST

	unique_lock<mutex> lock(this->queueMutex);
	for (int i = -CHK_RND_DIST; i < CHK_RND_DIST + 1; i++) {
		for (int j = -CHK_RND_DIST; j < CHK_RND_DIST + 1; j++){
			ChunkPos cp(pos + (int[]){i, j});
			// printf("surrounding pos: %d %d, %d, %d,   %lu\n", j, i, cp[0], cp[1], this->loadQueue.size());
			this->loadQueue.insert(cp);
		}
	}
}

ChunkPos	World::getCameraChunkPos()
{
	glm::vec3 mat;
	mat = this->enginePtr.getCam().getTranslate();
	float test[] = {mat.x / 16, mat.z / 16};
	if (test[0] < 0)
		test[0] -= 1.0;
	if (test[1] < 0)
		test[1] -= 1.0;
	return (int[]){(int)test[0], (int)test[1]};
}

Chunk	*World::get(ChunkPos cp)
{
	unique_lock<mutex> lock(this->memoryMutex);
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
	this->displayedMutex.lock();
	pair<unordered_set<ChunkPos>::iterator, bool> ret(this->getDisplayedChunks().begin(), false);
	this->displayedMutex.unlock();
	int		i = 0;

	if (chunk->getFenced())
	{
		{	unique_lock<mutex> lock(this->displayedMutex);
			ret = this->displayedChunks.insert(chunk->getPos()); 
		}
		if (ret.second)
			chunk->generateGraphics();
	}
	while (i < 4)
	{
		tmp = chunk->getNeighboor((Direction)i);
		if (tmp && tmp->getFenced())
		{
			{	unique_lock<mutex> lock(this->displayedMutex);
				ret = this->displayedChunks.insert(tmp->getPos());
			}
			if (ret.second)
				tmp->generateGraphics();
		}
		i++;
	}
}

void	World::loadChunk(ChunkPos cp)
{
	// printf(YELLOW "%d %d\n" NA, cp.get(0), cp.get(1));
	bool count;
	{	unique_lock<mutex> lock(this->memoryMutex);
		count = this->memoryChunks.count(cp);
	}
	if (count == 0)
	{
		Chunk	*newChunk = new Chunk(this, cp);
		this->worldGen.genChunk(newChunk);
		{
			unique_lock<mutex> lock(this->memoryMutex);
			this->memoryChunks[cp] = newChunk;
		}
		this->memoryChunks[cp]->updateFenced(1);
		this->pushInDisplay(newChunk);
	}
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
	unique_lock<mutex> lk(this->deltaFTMutex);
	return (this->deltaFrameTime);
}
