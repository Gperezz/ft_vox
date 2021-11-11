/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2021/11/11 11:15:41 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WORLD_HPP_
# define _WORLD_HPP_
# include <map>
# include <string>
# include <unordered_set>
# include <vector>
# include <set>
# include <thread>
# include <mutex>

# include "Engine.hpp"
# include "Chunk.hpp"
# include "WorldGenerator.hpp"
# define CHK_RND_DIST 10
# define CHK_DEL_DIST 10
# define CHK_DEL_DIST_MEM 4

using namespace std;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Engine;

class World
{
	private:
		bool					queueOn;
		thread					queueThread;
		mutex					queueMutex;
		mutex					memoryMutex;
		mutex					displayedMutex;
		set<ChunkPos, function<bool (ChunkPos, ChunkPos)>>	loadQueue;
		map<ChunkPos, Chunk*>	memoryChunks;
		// set<ChunkPos>			graphicQueue;
		unordered_set<ChunkPos>	displayedChunks;
		WorldGenerator			worldGen;
		Engine&					enginePtr;
		string					path;
		float					deltaFrameTime;
		float					lastFrameTime;
		// mutex					deltaFTMutex;
	
		void					pushInDisplay(Chunk* chunk, bool alreadyLoad);
	public:
							World(Engine& engine, unsigned long* = NULL);
							World(Engine& engine, string&, unsigned long* = NULL);
							// World(string pathStr, );
							// World(string )
							~World();

	
	void					initQueueThread();
	void					initQueueSorter();
	bool					LoadNextQueuedChunk();
	ChunkPos				getCameraChunkPos();
	void					rearrangeQueues();
	void					deleteFarInDisplay();
	void					deleteFar();
	void					display(Engine &e, float currentFrameTime);
	void					loadChunk(ChunkPos);
	void					loadChunk(int x, int z);
	Chunk					*get(ChunkPos);
	map<ChunkPos, Chunk*>	&getMapMemory(void);
	Chunk					*getMemoryChunk(ChunkPos pos);
	unordered_set<ChunkPos>	&getDisplayedChunks(void);
	float					getDeltaFrameTime(void);
	Chunk					*operator[](ChunkPos);
}; 

#endif