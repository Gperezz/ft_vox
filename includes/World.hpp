/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/11/28 00:35:07 by karldouveno      ###   ########.fr       */
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

# include "Chunk.hpp"
# include "WorldGenerator.hpp"
# define CHK_RND_DIST 16
# define CHK_DEL_DIST 32

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
		set<ChunkPos>			graphicQueue;
		unordered_set<ChunkPos>	displayedChunks;
		WorldGenerator			worldGen;
		Engine&					enginePtr;
		string					path;
		float					deltaFrameTime;
		float					lastFrameTime;
		mutex					deltaFTMutex;
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
	void					display(Engine &e, float currentFrameTime);
	void					pushInDisplay(Chunk* chunk);
	void					loadChunk(ChunkPos);
	void					loadChunk(int x, int z);
	Chunk					*get(ChunkPos);
	Chunk					*getMemoryChunk(ChunkPos pos);
	unordered_set<ChunkPos>	&getDisplayedChunks(void);
	float					getDeltaFrameTime(void);
	Chunk					*operator[](ChunkPos);
}; 

#endif