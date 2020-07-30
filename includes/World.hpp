/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/07/29 23:07:52 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WORLD_HPP_
# define _WORLD_HPP_
# include <map>
# include <filesystem>
# include <string>
# include <unordered_set>
# include <vector>
# include <queue>

# include "Chunk.hpp"
# include "WorldGenerator.hpp"

using namespace std;
using namespace std::filesystem;
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
		path						rootDirPath;
		WorldGenerator				worldGen;
		Mat							worldMatrix;
	public:
						World(unsigned long* = NULL);
						World(string&, unsigned long* = NULL);
						World(path&, unsigned long* = NULL);
						// World(string pathStr, );
						// World(string )
						~World();
	void				display(Engine &e);
	path				getDir();
	void				pushInDisplay(ChunkPos cp);
	void				loadChunk(ChunkPos);
	void				loadChunk(int x, int z);
	Mat					&getWorldMat(void);
	Chunk				*get(ChunkPos);
	Chunk				*getMemoryChunk(ChunkPos pos);
	vector<ChunkPos>	&getDisplayedChunks(void);
	Chunk				*operator[](ChunkPos);
}; 

#endif