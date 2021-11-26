/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2021/11/26 12:16:36 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"
#include <iostream>
using namespace std;

World::World(Engine& engine, unsigned long *seed)
:
	startGen(true),
	startLoad(true),
	start(true),
	enginePtr(engine),
	deltaFrameTime(0.0),
	lastFrameTime(0.0)
	{
		// initSet();
		{unique_lock<mutex>	lk(this->queueOnMutex);
			this->queueOn = true;
		}
		this->worldGen.configure(seed);
	}

World::~World()
{
	{unique_lock<mutex>	lk(this->memoryMutex);
		std::map<ChunkPos, Chunk*>::iterator it = memoryChunks.begin();
		while (it != memoryChunks.end())
		{
			delete it->second;
			it++;
		}
	}
}

bool		World::isEnd(void)
{
	return (!this->queueOn);
}

void		World::end(void)
{
	{unique_lock<mutex>	lk(this->queueOnMutex);
		queueOn = false;
	}
}

// void	World::initSet(void)
// {
// 	auto cmp = [this](ChunkPos a, ChunkPos b)->bool{
// 		ChunkPos center = this->getCameraChunkPos();
// 		// printf("center : %d, %d\n", center[0], center[1]);
// 		float da = a.distance(center);
// 		float db = b.distance(center);
// 		if (da == db)
// 			return a < b;
// 		return da < db;
// 	};
// 	this->graphicQueue = set<ChunkPos, function<bool (ChunkPos, ChunkPos)>>(cmp);
// }

bool		World::isStarted(void)
{
	return (this->start);
}

void	World::loopGen(bool value)
{
	bool test = true;
	while (test)
	{
		{unique_lock<mutex>	lk(queueOnMutex);
			if (!this->queueOn)
				return ;
		}
		this->insertGenQueue();
		while (this->genNextQueuedChunk())
			(void)this;
		test = value;
	}
}

void	World::loopLoad(bool value)
{
	bool test = true;
	while (test)
	{
		{unique_lock<mutex>	lk(queueOnMutex);
			if (!this->queueOn)
				return ;
		}
		this->insertLoadQueue();
		while (this->LoadNextQueuedChunk())
			(void)this;
		test = value;
	}
}

void	World::insertGenQueue(void)
{
	static ChunkPos	prevPos;
	int				renderDist = CHK_DIST_MEM;
	ChunkPos		pos = this->getCameraChunkPos();

	if (prevPos == pos && !this->startGen)
		return ;

	{unique_lock<mutex> lockStart(this->startGenMutex);
	this->startGen = false;}
	unique_lock<mutex> lk(this->genQueueMutex);
	for (int i = -renderDist; i < renderDist + 1; i++)
	{
		{unique_lock<mutex> lockStart(this->startGenMutex);
		if (this->startGen)
		{
			i = -renderDist;
			this->genQueue.clear();
		}}
		for (int j = -renderDist; j < renderDist + 1; j++)
		{
			int cpos[] = {i, j};
			ChunkPos cp(pos + cpos);
			if (this->genQueue.size() > this->genQueue.max_size() - 2)
			{
				std::cout << "MAX SIZE genQueue\n";
				return ;
			}
			if (this->genQueue.count(cp) == 0)
			{
				// std::cout << GREEN << this->genQueue.size() << " " << this->genQueue.max_size() << "\n" << NA;
				this->genQueue.insert(cp);
			}
		}
	}
	prevPos = pos;
}

void	World::insertLoadQueue(void)
{
	static ChunkPos	prevPos;
	int				renderDist = CHK_RND_DIST;
	ChunkPos		pos = this->getCameraChunkPos();

	if (prevPos == pos && !this->startLoad)
		return ;

	{unique_lock<mutex> lockStart(this->startLoadMutex);
	this->startLoad = false;}
	unique_lock<mutex> lk(this->queueMutex);
	for (int i = -renderDist; i < renderDist + 1; i++)
	{
		{unique_lock<mutex> lockStart(this->startLoadMutex);
		if (this->startLoad)
		{
			i = -renderDist;
			this->loadQueue.clear();
		}}
		for (int j = -renderDist; j < renderDist + 1; j++)
		{
			if (this->loadQueue.size() > this->loadQueue.max_size() - 2)
			{
				std::cout << "MAX SIZE loadQueue\n";
				return ;
			}
			int cpos[] = {i, j};
			ChunkPos cp(pos + cpos);
			if (this->loadQueue.count(cp) == 0)
			{
				// std::cout << BOLD_GREEN << this->loadQueue.size() << " " << this->loadQueue.max_size() << "\n" << NA;
				this->loadQueue.insert(cp);
			}
		}
	}
	prevPos = pos;
}

bool	World::isLoadable(ChunkPos &p)
{
	unique_lock<mutex> queueLock(this->queueMutex);
	size_t size;
	
	size = this->loadQueue.size();
	if (size)
	{
		auto pos = this->loadQueue.begin();
		p = *pos;
		this->loadQueue.erase(p);
		return true;
	}
	return false;
}

bool	World::isGen(ChunkPos &p)
{
	unique_lock<mutex> queueLock(this->genQueueMutex);
	size_t size;
	
	size = this->genQueue.size();
	if (size)
	{
		auto pos = this->genQueue.begin();
		p = *pos;
		this->genQueue.erase(p);
		return true;
	}
	return false;
}

unsigned int		World::genNextQueuedChunk(void)
{
	ChunkPos		pos;

	if (isGen(pos))
		this->genChunk(pos);
	size_t size;
	{unique_lock<mutex> genQueueLock(this->genQueueMutex);
		size = this->genQueue.size();
	}
	return (size);
}

unsigned int		World::LoadNextQueuedChunk(void)
{
	ChunkPos		pos;

	if (isLoadable(pos))
		this->loadChunk(pos);
	size_t size;
	{unique_lock<mutex> queueLock(this->queueMutex);
		size = this->loadQueue.size();
	}
	return (size);
}

void	World::loadChunk(ChunkPos cp)
{
	unique_lock<mutex> lockMem(this->memoryMutex); // On check tout au long de la fonction si le chunk existe encore dans memory
	{unique_lock<mutex> lockDisp(this->displayedMutex);
	if (this->displayedChunks.count(cp))
		return;
	} // Check s'il est deja afficher

	std::map<ChunkPos, Chunk*>::iterator findChunk = this->memoryChunks.find(cp);
	if (findChunk != this->memoryChunks.end())
	{
		{unique_lock<mutex> lockDisp(this->displayedMutex);
			if (this->displayedChunks.count(cp))
				return;
			// findChunk->second->updateFencedUnsafe(1);
			this->pushInDisplay(findChunk->second, findChunk->second->isGenerated());
		}
		return;
	}
}

void	World::genChunk(ChunkPos cp)
{
	{unique_lock<mutex> lockMem(this->memoryMutex);
	if (this->memoryChunks.count(cp))
		return ;
	}
	
	if (this->memoryChunks.size() > this->memoryChunks.max_size() - 2)
	{
		std::cout << "MAX SIZE MEMORY\n";
		return ;
	}

	Chunk	*newChunk = NULL;
	newChunk = new Chunk(this, cp);
	if (!newChunk)
	{
		this->end();
		return;
	}
	this->worldGen.genChunk(newChunk);
	
	{unique_lock<mutex> lockMem(this->memoryMutex);
		this->memoryChunks.insert(std::pair<ChunkPos, Chunk*>(cp, newChunk));
		// std::cout << GREEN << cp.get(0) << " " << cp.get(1) << "\n" << NA;
		this->memoryChunks[cp]->updateFencedUnsafe(1);
	}
}

void	World::pushInDisplay(Chunk* chunk, bool alreadyGen)
{
	ChunkPos	chunkP;

	if (!chunk)
		return;
	chunkP = chunk->getPos();

	if (chunk->getFenced())
	{
		if (this->displayedChunks.size() > this->displayedChunks.max_size() - 2)
		{
			std::cout << "MAX SIZE DISPLAY\n";
			return ;
		}
		this->displayedChunks.insert(chunkP);
		if (!alreadyGen)
		{
			unique_lock<mutex> lk(this->graphicMutex); //chunk->generateGraphics();
			if (this->graphicQueue.size() > this->graphicQueue.max_size() - 2)
			{
				std::cout << "MAX SIZE GRAPHICS\n";
				return ;
			}
			this->graphicQueue.insert(chunkP);
		}
	}
	// Chunk*	tmp;
	// int		i = 0;
	// while (i < 4 && !alreadyGen)
	// {
	// 	tmp = chunk->getNeighboorUnsafe((Direction)i);
	// 	if (tmp && tmp->getFenced())
	// 	{
	// 		this->displayedChunks.insert(tmp->getPos());
	// 		{unique_lock<mutex> lk(this->graphicMutex); //tmp->generateGraphics();
	// 			this->graphicQueue.insert(tmp);
	// 		}
	// 	}
	// 	i++;
	// }
}

void	World::loadGraphics(void)
{
	std::map<ChunkPos, Chunk*>::iterator	chunk;
	unique_lock<mutex> lockMem(this->memoryMutex);
	unique_lock<mutex> lockGraph(this->graphicMutex);
	if (!this->graphicQueue.size())
	{
		this->start = false;
		return ;
	}

	for (set<ChunkPos>::iterator it = this->graphicQueue.begin(); it != this->graphicQueue.end(); void(it))
	{
		int size = this->graphicQueue.size();
		if ((chunk = this->memoryChunks.find(*it)) == this->memoryChunks.end())
			return;
		chunk->second->generateGraphics();
		it = this->graphicQueue.erase(it);
		if (size < CHK_SAFE_DIST)
			return;
	}
}

void	World::deleteFarInDisplay()
{
	static ChunkPos			prevPos;
	ChunkPos				chunkP;
	ChunkPos				pos = this->getCameraChunkPos();

	if (pos == prevPos)
		return ;

	{unique_lock<mutex> lock(this->displayedMutex);
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end();)
	{
		chunkP = *it;
		if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST 
			|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST
			|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST 
			|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST)
				it = this->displayedChunks.erase(it);
		else
			it++;
	}}
	
	{unique_lock<mutex> lock(this->genQueueMutex);
	for (auto it = this->genQueue.begin(); it != this->genQueue.end();)
	{
		chunkP = *it;
		if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST 
			|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST
			|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST 
			|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST)
				it = this->genQueue.erase(it);
		else
			it++;
	}}

	{unique_lock<mutex> lock(this->queueMutex);
	for (auto it = this->loadQueue.begin(); it != this->loadQueue.end();)
	{
		chunkP = *it;
		if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST 
			|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST
			|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST 
			|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST)
				it = this->loadQueue.erase(it);
		else
			it++;
	}}

	{unique_lock<mutex> lock(this->graphicMutex);
	for (auto it = this->graphicQueue.begin(); it != this->graphicQueue.end();)
	{
		chunkP = *it;
		if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST 
			|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST
			|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST 
			|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST)
				it = this->graphicQueue.erase(it);
		else
			it++;
	}}

}

void	World::deleteFar(void)
{
	static			ChunkPos	prevPos;
	ChunkPos		pos = this->getCameraChunkPos();
	ChunkPos		chunkP;

	if (pos == prevPos)
		return ;
	unique_lock<mutex> lockMem(this->memoryMutex);
	for (auto it = this->memoryChunks.begin(); it != this->memoryChunks.end();)
	{
		chunkP = it->first;
		if (chunkP.get(0) - pos.get(0) > CHK_DEL_DIST_MEM 
			|| chunkP.get(0) - pos.get(0) < -CHK_DEL_DIST_MEM
			|| chunkP.get(1) - pos.get(1) > CHK_DEL_DIST_MEM 
			|| chunkP.get(1) - pos.get(1) < -CHK_DEL_DIST_MEM)
		{
			Chunk* delChunk = it->second;
			delChunk->updateDelFenced();
			it = this->memoryChunks.erase(it);
			delete delChunk;
		}
		else
			it++;
	}
	pos = prevPos;
}

void	World::queueToDisplay(void)
{
	this->loadGraphics();
	this->deleteFarInDisplay();
	this->deleteFar();
}

void	World::display(Engine &e, float currentFrameTime)
{
	this->queueToDisplay();
	if (e.isSkybox() && e.getTexture(1))
		e.displaySky(e.getTexture(1));
	
	Textures	*texture = e.getTexture(0);
	Shader		&shader = e.getShader();
	Camera		cam = e.getCam();
	
	unique_lock<mutex> lk3(this->memoryMutex);
	unique_lock<mutex> lk(this->displayedMutex);
	std::map<ChunkPos, Chunk*>::iterator	chunk;
	for (auto it = this->displayedChunks.begin(); it != this->displayedChunks.end(); it++)
	{
		chunk = this->memoryChunks.find(*it);
		if (chunk != this->memoryChunks.end() && chunk->second->isGenerated())
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
	int cpos[] = {(int)cam[0], (int)cam[1]};
	return (ChunkPos(cpos));
}

Chunk	*World::get(ChunkPos cp)
{
	unique_lock<mutex>	lk(this->memoryMutex);
	if (this->memoryChunks.count(cp) != 1)
		return NULL;
	return this->memoryChunks.at(cp);
}

Chunk	*World::getUnsafe(ChunkPos cp)
{
	if (this->memoryChunks.count(cp) != 1)
		return NULL;
	return this->memoryChunks.at(cp);
}

Chunk	*World::operator[](ChunkPos cp)
{
	return this->get(cp);
}

// Chunk	*World::getMemoryChunk(ChunkPos pos)
// {
// 	return (this->memoryChunks.at(pos));
// }

// set<ChunkPos>	&World::getDisplayedChunks(void)
// {
// 	return (this->displayedChunks);
// }

// void	World::loadChunk(int x, int z)
// {
// 	return (this->loadChunk(ChunkPos((int[2]){x, z})));
// }

float	World::getDeltaFrameTime(void)
{
	return (this->deltaFrameTime);
}
