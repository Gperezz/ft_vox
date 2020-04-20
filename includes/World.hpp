/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/04/20 00:25:05 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WORLD_HPP_
# define _WORLD_HPP_
# include <map>
# include <filesystem>
# include <string>
# include <set>
# include <vector>

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
		set<ChunkPos>				loadQueue;
		map<ChunkPos, Chunk>		memoryChunks;
		set<ChunkPos>				graphicQueue;
		vector<ChunkPos>			displayedChunks;
		path						rootDirPath;
		WorldGenerator				worldGen;
	public:
			World(unsigned long* = NULL);
			World(string&, unsigned long* = NULL);
			World(path&, unsigned long* = NULL);
			// World(string pathStr, );
			// World(string )
			~World();
	void	display(Engine &e);
	path	getDir();
	void	loadChunk(ChunkPos);
	void	loadChunk(int x, int z);
	Chunk	*get(ChunkPos);
	Chunk	*operator[](ChunkPos);
}; 

#endif