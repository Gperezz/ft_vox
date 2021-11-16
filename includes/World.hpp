/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2021/11/16 15:48:39 by gperez           ###   ########.fr       */
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
# define CHK_RND_DIST 4
# define CHK_DEL_DIST 6
# define CHK_DEL_DIST_MEM 8

using namespace std;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Engine;

class World
{
	private:
		bool					queueOn;
		mutex					queueMutex;
		mutex					memoryMutex;
		mutex					displayedMutex;
		mutex					graphicMutex;
		set<ChunkPos, function<bool (ChunkPos, ChunkPos)>>	loadQueue;
		map<ChunkPos, Chunk*>	memoryChunks;
		unordered_set<ChunkPos>	displayedChunks;
		unordered_set<Chunk*>	graphicQueue;
		WorldGenerator			worldGen;
		Engine&					enginePtr;
		string					path;
		float					deltaFrameTime;
		float					lastFrameTime;
		// mutex					deltaFTMutex;
	
		bool					isLoadable(ChunkPos &pos);
		void					pushInDisplay(Chunk* chunk, bool alreadyLoad);
		void					loadGraphics(void);

	public:
							World(Engine& engine, unsigned long* = NULL);
							World(Engine& engine, string&, unsigned long* = NULL);
							// World(string pathStr, );
							// World(string )
							~World();

	void					test();
	void					test2();
	void					initQueueThread();
	void					initQueueSorter();
	void					LoadNextQueuedChunk();
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