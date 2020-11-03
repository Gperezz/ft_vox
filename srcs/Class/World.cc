/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/11/03 02:15:13 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"
#include <iostream>
using namespace std;

World::World(unsigned long *seed)
{
	this->queueOn = true;
	this->initQueueSorter();
	//this->initQueueThread();
	this->worldGen.configure(seed);
}

World::World(string& path, unsigned long *seed)
{
	this->queueOn = true;
	this->initQueueSorter();
	//this->initQueueThread();
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
	this->queueOn = false;
	queueThread.join();
}

void	World::initQueueSorter()
{
	auto cmp = [this](ChunkPos a, ChunkPos b)->bool{
		ChunkPos center = this->getCameraChunkPos();
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
		printf("something's up");
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

void	World::initQueueThread()
{
	auto queueLoop = [this]() {
		while (this->queueOn){
			this->LoadNextQueuedChunk();
		}
	};
	this->queueThread = thread(queueLoop);
}

void	World::display(Engine &e)
{
	this->rearrangeQueues();
	this->LoadNextQueuedChunk();
	if (e.isSkybox() && e.getTexture(SKY_T - END_BLOCK_T))
		e.displaySky(e.getTexture(SKY_T - END_BLOCK_T));
	unique_lock<mutex> lock(this->displayedMutex);
	unique_lock<mutex> lock2(this->matMutex);
	unique_lock<mutex> lock3(this->memoryMutex);
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
		this->memoryChunks.at(*it)->displayChunk(e, this->getWorldMat().getMatrix(true));
}

void	World::rearrangeQueues()
{
	static ChunkPos pos;
	ChunkPos newPos = this->getCameraChunkPos();
	printf("cur pos: %d %d\n", pos[0], pos[1]);

	if (pos == newPos)
		return ;
	pos = newPos;
	unique_lock<mutex> lock(this->queueMutex);
	for (int i = -CHK_RND_DIST; i < CHK_RND_DIST + 1; i++) {
		for (int j = -CHK_RND_DIST; j < CHK_RND_DIST + 1; j++){
			ChunkPos cp(pos + (int[]){i, j});
			printf("surrounding pos: %d %d, %d, %d,   %lu\n", j, i, cp[0], cp[1], this->loadQueue.size());

			this->loadQueue.insert(cp);
		}
	}
}

ChunkPos	World::getCameraChunkPos()
{

	glm::vec3 mat;
	{
		unique_lock<mutex> lock(this->matMutex);
		mat = this->getWorldMat().getTranslate();
	}
	float test[] = {-mat.x / 16, -mat.z / 16};
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
	printf(YELLOW "%d %d\n" NA, cp.get(0), cp.get(1));
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

mutex	&World::getMatMutex()
{
	return this->matMutex;
}	