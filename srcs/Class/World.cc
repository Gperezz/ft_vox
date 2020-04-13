/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cc                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:13:57 by gperez            #+#    #+#             */
/*   Updated: 2020/04/13 19:38:16 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

using namespace std;
using namespace std::filesystem;

World::World(void)
{
	
}

World::World(string pathStr)
{
}

World::World(path path)
{

}

// World::World(string pathStr,)

World::~World()
{
}

path	World::getDir(){
	return this->RootDirPath;
}
void	World::loadChunk(ChunkPos cp){
	if (this->LoadedChunks.find(cp) == this->LoadedChunks.end()){
		Chunk	*newChunk = new Chunk(this, cp);
		this->WorldGen.genChunk(newChunk);
		this->LoadedChunks[cp] = newChunk;
		validateChunk();
	}
}
void	World::loadChunk(unsigned x, unsigned z);
