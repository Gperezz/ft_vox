/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:08:20 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 19:10:22 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP
# include <map>
# include <filesystem>
# include <string>

# include "Chunck.hpp"

using namespace std;
using namespace std::filesystem;

class Chunk;
class WorldGenerator;


class World
{
	private:
		map<ChunkPos, Chunk>&		LoadedChunks;
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

};

#endif // WORLD_HPP