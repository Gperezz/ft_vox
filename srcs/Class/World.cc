/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/04/19 18:56:43 by gperez           ###   ########.fr       */
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
}

void	World::display(Engine &e)
{
	size_t	i;

	i = 0;
	while (i < this->displayedChunks.size())
	{
		this->memoryChunks.at(displayedChunks[i]).displayChunk(e);
		i++;
	}
}

path	World::getDir(){
	return this->rootDirPath;
}

Chunk	*World::get(ChunkPos cp)
{
	if (this->memoryChunks.count(cp) == 0)
		return NULL;
	return &this->memoryChunks.at(cp); // Potentiellement optimisable
}

Chunk	*World::operator[](ChunkPos cp)
{
	return this->get(cp);
}

void	World::loadChunk(ChunkPos cp)
{
	if (this->memoryChunks.count(cp) == 0)
	{
		Chunk	newChunk = Chunk(this, cp);
		this->worldGen.genChunk(newChunk);
		printf("avant\n");
		this->memoryChunks[cp] = newChunk;
		printf("apres\n");
		// this->memoryChunks[cp].printSlice(0);
		newChunk.printSlice(0);
		newChunk.updateFenced();
		newChunk.generateGraphics();
		displayedChunks.push_back(newChunk.getPos()); // displayQueue
	}
}

void	World::loadChunk(int x, int z)
{
	return this->loadChunk(ChunkPos((int[2]){x, z}));
}
