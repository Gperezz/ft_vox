/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2021/11/17 17:36:08 by gperez           ###   ########.fr       */
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
# define CHK_DEL_DIST 5
# define CHK_DEL_DIST_MEM 6

using namespace std;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Engine;

class World
{
	private:
		bool					queueOn;
		mutex					queueOnMutex;
		mutex					queueMutex;
		mutex					memoryMutex;
		mutex					displayedMutex;
		mutex					graphicMutex;
		mutex					chunkPMutex;
		set<ChunkPos, function<bool (ChunkPos, ChunkPos)>>	loadQueue;
		map<ChunkPos, Chunk*>	memoryChunks;
		unordered_set<ChunkPos>	displayedChunks;
		unordered_set<Chunk*>	graphicQueue;
		unordered_set<ChunkPos>	chunkPQueue;
		WorldGenerator			worldGen;
		Engine&					enginePtr;
		string					path;
		float					deltaFrameTime;
		float					lastFrameTime;
		// mutex					deltaFTMutex;
	


		void					initQueueSorter(void);

		void					insertLoadQueue(void);
		bool					isLoadable(ChunkPos &pos);
		void					parallelizeLoad(void);
		unsigned int			LoadNextQueuedChunk(void);
		void					loadChunk(ChunkPos);
		void					pushInDisplay(Chunk* chunk);
		void					loadGraphics(void);

		void					deleteFarInDisplay(void);
		void					deleteFar(void);

	public:
							World(Engine& engine, unsigned long* = NULL);
							World(Engine& engine, string&, unsigned long* = NULL);
							// World(string pathStr, );
							// World(string )
							~World();

	void const				initThread(void);
	void					queueToDisplay(void);
	void					display(Engine &e, float currentFrameTime);
	ChunkPos				getCameraChunkPos();
	// void					loadChunk(int x, int z);
	Chunk					*get(ChunkPos);
	Chunk					*getUnsafe(ChunkPos);
	// Chunk					*getMemoryChunk(ChunkPos pos);
	unordered_set<ChunkPos>	&getDisplayedChunks(void);
	float					getDeltaFrameTime(void);
	Chunk					*operator[](ChunkPos);
	void					end(void);
};

#endif