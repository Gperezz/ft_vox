/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/04/17 17:14:11 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WORLD_HPP_
# define _WORLD_HPP_
# include <map>
# include <filesystem>
# include <string>

# include "Chunk.hpp"
# include "WorldGenerator.hpp"

using namespace std;
using namespace std::filesystem;
using ChunkPos = Coords::Coords<int, 2>;
using BlockPos = Coords::Coords<int, 4>;

class Chunk;
class WorldGenerator;

class World
{
	private:
		map<ChunkPos, Chunk>		*LoadedChunks;
		path						*RootDirPath;
		WorldGenerator				*WorldGen;
	public:
			World(unsigned long* = NULL);
			World(string&, unsigned long*);
			World(path&, unsigned long*);
			// World(string pathStr, );
			// World(string )
			~World();
	path	*getDir();
	void	loadChunk(ChunkPos);
	void	loadChunk(int x, int z);
	Chunk&	get(ChunkPos);
	Chunk&	operator[](ChunkPos);
};

#endif