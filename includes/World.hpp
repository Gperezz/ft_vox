/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/10/09 16:58:03 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WORLD_HPP_
# define _WORLD_HPP_
# include <map>
# include <string>
# include <unordered_set>
# include <vector>
# include <queue>

# include "Chunk.hpp"
# include "WorldGenerator.hpp"

using namespace std;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Engine;

class World
{
	private:
		queue<ChunkPos>				loadQueue;
		map<ChunkPos, Chunk*>		memoryChunks;
		queue<ChunkPos>				graphicQueue;
		unordered_set<ChunkPos>		displayedChunks;
		WorldGenerator				worldGen;
		Mat							worldMatrix;
		string						path;
	public:
							World(unsigned long* = NULL);
							World(string&, unsigned long* = NULL);
							// World(string pathStr, );
							// World(string )
							~World();
	void					display(Engine &e);
	void					pushInDisplay(Chunk* chunk);
	void					loadChunk(ChunkPos);
	void					loadChunk(int x, int z);
	Mat						&getWorldMat(void);
	Chunk					*get(ChunkPos);
	Chunk					*getMemoryChunk(ChunkPos pos);
	unordered_set<ChunkPos>	&getDisplayedChunks(void);
	Chunk					*operator[](ChunkPos);
}; 

#endif