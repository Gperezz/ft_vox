/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunck.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 18:17:27 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 19:34:50 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHUNCK_HPP
# define _CHUNCK_HPP

# include "Coords.hpp"
# include "Block.hpp"

enum xz_vec {X, Z};
using ChunkPos = Coords::Coords<int, 2>;

class Chunk{
private:
	Block		blocks[16][256][16];
	
};
#endif