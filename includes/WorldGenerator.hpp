/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/30 16:22:18 by gperez            #+#    #+#             */
/*   Updated: 2020/04/20 19:32:51 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_GENERATOR_HPP
# define WORLD_GENERATOR_HPP
# include "Rand.hpp"
# include "Block.hpp"
# include "Chunk.hpp"

class WorldGenerator{
	private:
		unsigned long seed;
	public:
		WorldGenerator();
		WorldGenerator(unsigned long* seed);

		void genChunk(Chunk*);
		void configure(unsigned long* seed);
};
#endif