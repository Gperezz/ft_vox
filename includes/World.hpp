/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/04/14 21:46:41 by gperez           ###   ########.fr       */
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

class World
{
	private:
		map<ChunkPos, Chunk*>&		LoadedChunks;
		path&						RootDirPath;
		WorldGenerator&				WorldGen;
	public:
			World(void);
			World(string pathStr);
			World(path path);
			// World(string pathStr, );
			// World(string )
			~World();
	path	getDir();
	void	loadChunk(ChunkPos);
	void	loadChunk(unsigned x, unsigned z);
	Chunk	*operator[](ChunkPos);
};

#endif