/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2021/11/29 14:32:16 by gperez           ###   ########.fr       */
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
# include "WorldGenerator.hpp"
# define CHK_RND_DIST 14//10//7//14
# define CHK_DEL_DIST 18//14//9//18
# define CHK_DIST_MEM 20//10//20
# define CHK_DEL_DIST_MEM 25//15//30

# define CHK_SAFE_DIST CHK_RND_DIST * CHK_RND_DIST

using namespace std;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Engine;

class World
{
	private:
		mutex					startGenMutex;
		bool					startGen;
		mutex					startLoadMutex;
		bool					startLoad;
		bool					start;
		bool					queueOn;
		mutex					queueOnMutex;
		mutex					genQueueMutex;
		mutex					queueMutex;
		mutex					memoryMutex;
		mutex					displayedMutex;
		mutex					graphicMutex;
		set<ChunkPos>			genQueue;
		set<ChunkPos>			loadQueue;
		map<ChunkPos, Chunk*>	memoryChunks;
		set<ChunkPos>			displayedChunks;
		set<ChunkPos>			graphicQueue;
		WorldGenerator			worldGen;
		Engine&					enginePtr;
		string					path;
		float					deltaFrameTime;
		float					lastFrameTime;
	

		void					initSet(void);

		void					insertGenQueue(void);
		void					insertLoadQueue(void);
		bool					isLoadable(ChunkPos &pos);
		bool					isGen(ChunkPos &pos);
		unsigned int			LoadNextQueuedChunk(void);
		unsigned int			genNextQueuedChunk(void);
		void					loadChunk(ChunkPos cp);
		void					genChunk(ChunkPos cp);
		void					pushInDisplay(Chunk* chunk, bool alreadyGen);

		void					deleteFarInDisplay(void);
		void					deleteFar(void);

	public:
							World(Engine& engine, unsigned long* = NULL);
							~World();

	void					loopGen(bool value);
	void					loopLoad(bool value);
	void					loadGraphics(void);
	void					queueToDisplay(void);
	void					display(Engine &e, float currentFrameTime);
	ChunkPos				getCameraChunkPos();
	Chunk					*get(ChunkPos);
	Chunk					*getUnsafe(ChunkPos);
	float					getDeltaFrameTime(void);
	Chunk					*operator[](ChunkPos);
	void					end(void);
	bool					isEnd(void);
	bool					isStarted(void);
};

#endif