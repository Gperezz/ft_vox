/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/30 16:22:18 by gperez            #+#    #+#             */
/*   Updated: 2020/04/13 16:55:24 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_GENERATOR_HPP
# define WORLD_GENERATOR_HPP
# include "Rand.hpp"
# include "Chunk.hpp"
# include "Block.hpp"

class Chunk;

class WorldGenerator{
private:
	unsigned long seed;
public:
	WorldGenerator(unsigned long seed);
	WorldGenerator(void);

	void GenChunk(Chunk);
}
#endif