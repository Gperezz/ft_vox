/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2021/11/16 15:49:55 by gperez           ###   ########.fr       */
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

bool	World::isLoadable(ChunkPos &p)
{
	unique_lock<mutex> queueLock(this->queueMutex);
	size_t size;
	
	size = this->loadQueue.size();
	if (size)
	{
		decltype(this->loadQueue.begin()) pos;
		pos = this->loadQueue.begin();
		this->loadQueue.erase(*pos);
		p = *pos;
		return true;
	}
	return false;
}

void	World::LoadNextQueuedChunk()
{
	std::thread t1 ([this](){
		ChunkPos		pos;
		if (isLoadable(pos))
			this->loadChunk(pos);
	});

	// std::thread t2 ([this](){
	// 	ChunkPos		pos;
	// 	if (isLoadable(pos))
	// 		this->loadChunk(pos);
	// });

	// std::thread t3 ([this](){
	// 	ChunkPos		pos;
	// 	if (isLoadable(pos))
	// 		this->loadChunk(pos);
	// });

	// std::thread t4 ([this](){
	// 	ChunkPos		pos;
	// 	if (isLoadable(pos))
	// 		this->loadChunk(pos);
	// });

	if (t1.joinable())
		t1.join();
	// if (t2.joinable())
	// 	t2.join();
	// if (t3.joinable())
	// 	t3.join();
	// if (t4.joinable())
	// 	t4.join();
}

void	World::deleteFar()
{
	static ChunkPos	prevPos;
	ChunkPos		pos = this->getCameraChunkPos();
	ChunkPos		chunkP;

	if (pos == prevPos)
		return ;
	//unique_lock<mutex> lockMem(this->memoryMutex);
	for (auto it = this->memoryChunks.begin(); it != this->memoryChunks.end(); it++)
	{
		chunkP = it->first;
		if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST_MEM 
			|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST_MEM
			|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST_MEM 
			|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST_MEM)
		{
		Chunk* delChunk = this->memoryChunks.at(chunkP);
		// 	std::cout << RED << "Chunk " << chunkP.get(0) << " " << chunkP.get(1) << "\n";
		// 	std::cout << GREEN << this->memoryChunks.count(chunkP) << "\n";

			delete delChunk;
		// 	this->memoryChunks[chunkP]->updateDelFenced();
		// 	this->memoryChunks[chunkP] = NULL;
			this->memoryChunks.erase(chunkP);
		}
	}
	pos = prevPos;
}

void	World::deleteFarInDisplay()
{
	static ChunkPos	prevPos;
	ChunkPos		chunkP;
	ChunkPos		pos = this->getCameraChunkPos();

	if (pos == prevPos)
		return ;

	{
		unique_lock<mutex> lockQueue(this->queueMutex);
		for (auto it = this->loadQueue.begin(); it != this->loadQueue.end(); it++)
		{
			chunkP = *it;
			if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST 
				|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST
				|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST 
				|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST)
				{
					this->loadQueue.erase(chunkP);
				}
		}
	}

	{
		unique_lock<mutex> lock(this->displayedMutex);
		for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
		{
			chunkP = *it;
			if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST 
				|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST
				|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST 
				|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST)
					this->displayedChunks.erase(it);
		}
	}

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
	std::cout << BLUE << "Chunk " << pos.get(0) << " " << pos.get(1) << "\n";

	unique_lock<mutex> lk(this->queueMutex);
	for (int i = -CHK_RND_DIST; i < CHK_RND_DIST + 1; i++)
	{
		for (int j = -CHK_RND_DIST; j < CHK_RND_DIST + 1; j++)
		{
			ChunkPos cp(pos + (int[]){i, j});
			if (this->loadQueue.count(cp) == 0)
			{
				this->loadQueue.insert(cp);
			}
		}
	}
	prevPos = pos;
}

void	World::loadGraphics(void)
{
	unique_lock<mutex> lockGraph(this->graphicMutex);
	for (auto it = this->graphicQueue.begin(); it != this->graphicQueue.end(); it++)
		(*it)->generateGraphics();
	this->graphicQueue.clear();
}

void	World::display(Engine &e, float currentFrameTime)
{
	this->rearrangeQueues();

	// std::thread t1 ([this](){
		// this->deleteFarInDisplay();
	// });
	
	// std::thread t1 ([this](){
		this->LoadNextQueuedChunk(); //essayer de le commenter
	// });

	// t1.join();
	// t2.join();

	this->loadGraphics();

	// this->deleteFar();

	if (e.isSkybox() && e.getTexture(1))
		e.displaySky(e.getTexture(1));
	// unique_lock<mutex> lk(this->displayedMutex);
	// unique_lock<mutex> lk2(this->deltaFTMutex);
	// unique_lock<mutex> lk3(this->memoryMutex);
	
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
	float cam[] = {mat.x / (float)16., mat.z / (float)16.};
	if (cam[0] < 0.)
		cam[0] -= 1.0;
	if (cam[1] < 0.)
		cam[1] -= 1.0;
	return (int[]){(int)cam[0], (int)cam[1]};
}

Chunk	*World::get(ChunkPos cp)
{
	if (this->memoryChunks.count(cp) != 1)
		return NULL;
	return this->memoryChunks.at(cp);
}

Chunk	*World::operator[](ChunkPos cp)
{
	return this->get(cp);
}

void	World::pushInDisplay(Chunk* chunk, bool alreadyLoad)
{
	ChunkPos chunkP;

	pair<unordered_set<ChunkPos>::iterator, bool> ret(this->displayedChunks.begin(), false);

	if (!chunk)
		return;
	chunkP = chunk->getPos();

	if (chunk->getFenced())
	{
		ret = this->displayedChunks.insert(chunk->getPos());
		if (ret.second && !alreadyLoad)
		{
			{unique_lock<mutex> lk(this->graphicMutex); //chunk->generateGraphics();
				this->graphicQueue.insert(chunk);
			}
		}
	}
	Chunk*	tmp;
	int		i = 0;
	while (i < 4 && !alreadyLoad)
	{
		tmp = chunk->getNeighboor((Direction)i);
		if (tmp && tmp->getFenced())
		{
			ret = this->displayedChunks.insert(tmp->getPos());
			if (ret.second)
			{
				{unique_lock<mutex> lk(this->graphicMutex); //tmp->generateGraphics();
					this->graphicQueue.insert(tmp);
				}
			}
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

	{
		Chunk	*newChunk = NULL;
		unique_lock<mutex> lockMem(this->memoryMutex); // On check tout au long de la fonction si le chunk existe encore dans memory
		std::map<ChunkPos, Chunk*>::iterator findChunk = this->memoryChunks.find(cp);
		if (findChunk != this->memoryChunks.end())
		{
			newChunk = findChunk->second;
			{unique_lock<mutex> lockDisp(this->displayedMutex);
				if (this->displayedChunks.count(cp))
					return;
				this->pushInDisplay(newChunk, true);
			}
			return;
		}
	}
	Chunk	*newChunk = NULL;
	newChunk = new Chunk(this, cp);
	this->worldGen.genChunk(newChunk);
	
	{unique_lock<mutex> lockMem(this->memoryMutex);
		if (this->memoryChunks.find(cp) != this->memoryChunks.end()) // Si le chunk a ete ajouter entre temps
		{
			std::cout << "Here\n";
			delete newChunk;
			return;
		}
		this->memoryChunks.insert(std::pair<ChunkPos, Chunk*>(cp, newChunk));
		this->memoryChunks[cp]->updateFenced(1);
	
		{unique_lock<mutex> lockDisp(this->displayedMutex);
			if (this->displayedChunks.count(cp))
					return;
			this->pushInDisplay(newChunk, false);
		}
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
	// unique_lock<mutex> lk(this->deltaFTMutex);
	return (this->deltaFrameTime);
}
