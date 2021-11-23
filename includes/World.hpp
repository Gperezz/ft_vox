/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2021/11/23 14:05:39 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WORLD_HPP_
# define _WORLD_HPP_
# include <map>
# include <string>
# include <unordered_set>
# include <vector>
# include <set>

# include "Engine.hpp"
# include "Chunk.hpp"
# include "WorldGenerator.hpp"
# define CHK_RND_DIST 10
# define CHK_DEL_DIST 15
# define CHK_DIST_MEM 20
# define CHK_DEL_DIST_MEM 30

using namespace std;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Engine;

class World
{
	private:
		bool					queueOn;
		mutex					queueOnMutex;
		mutex					genQueueMutex;
		mutex					queueMutex;
		mutex					memoryMutex;
		mutex					displayedMutex;
		mutex					graphicMutex;
		mutex					chunkPMutex;
		set<ChunkPos>			genQueue;
		set<ChunkPos>			loadQueue;
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

		void					insertGenQueue(void);
		void					insertLoadQueue(void);
		bool					isLoadable(ChunkPos &pos);
		bool					isGen(ChunkPos &pos);
		unsigned int			LoadNextQueuedChunk(void);
		unsigned int			genNextQueuedChunk(void);
		void					loadChunk(ChunkPos cp);
		void					genChunk(ChunkPos cp);
		void					pushInDisplay(Chunk* chunk, bool alreadyGen);
		void					loadGraphics(void);

		void					deleteFarInDisplay(void);
		void					deleteFar(void);

	public:
							World(Engine& engine, unsigned long* = NULL);
							World(Engine& engine, string&, unsigned long* = NULL);
							// World(string pathStr, );
							// World(string )
							~World();

	void					loopGen(bool value);
	void					loopLoad(bool value);
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