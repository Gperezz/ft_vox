/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/04/17 17:13:52 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

using namespace std;
using namespace std::filesystem;

World::World(unsigned long *seed = NULL)
{
	this->LoadedChunks = new map<ChunkPos, Chunk>();
	this->RootDirPath = NULL;
	this->WorldGen = new WorldGenerator(seed);
}

World::World(string& pathStr, unsigned long *seed = NULL)
{
	this->LoadedChunks = new map<ChunkPos, Chunk>();
	this->RootDirPath = new path(pathStr);
	this->WorldGen = new WorldGenerator(seed);
}

World::World(path& p, unsigned long *seed = NULL)
{
	this->LoadedChunks = new map<ChunkPos, Chunk>();
	this->RootDirPath = new path(p);
	this->WorldGen = new WorldGenerator(seed);
}

path	*World::getDir(){
	return this->RootDirPath;
}

Chunk&	World::get(ChunkPos cp)
{
	return this->LoadedChunks->at(cp);
}

Chunk&	World::operator[](ChunkPos cp)
{
	return this->get(cp);
}

void	World::loadChunk(ChunkPos cp){
	if (this->LoadedChunks->find(cp) == this->LoadedChunks->end())
	{
		Chunk	newChunk = Chunk(this, cp);
		this->WorldGen->genChunk(newChunk);
		this->LoadedChunks->at(cp) = newChunk;
		newChunk.validateChunk();
	}
}

void	World::loadChunk(int x, int z)
{
	return this->loadChunk(ChunkPos((int[2]){x, z}));
}
