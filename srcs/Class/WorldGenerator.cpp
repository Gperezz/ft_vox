/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldGenerator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/02 08:06:26 by gperez            #+#    #+#             */
/*   Updated: 2020/04/02 08:06:45 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldGenerator.hpp"

	WorldGenerator::WorldGenerator(unsigned long seed) : seed(seed)
	{}
	WorldGenerator::WorldGenerator(void) : seed(Rand<unsigned long>().generate())
	{}

	WorldGenerator::GenChunk(Chunk chunk){
		for (int i = 0; i < 16 * 16 * 256; i++){
			chunk.get(i) = (t_block_info){1, 0, 0, 0};
		}
	}