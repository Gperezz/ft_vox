/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Coords.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 20:07:59 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 19:28:17 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COORDS_HPP
# define COORDS_HPP

namespace Coords{

template <typename T, unsigned dims>
struct Coords
{
private:
	T	attr[dims];

public:
	Coords(T attr[dims]){
		this->attr = attr;
	}

	Coords(Coords<T, dims> &toClone){
		this->attr = toClone.attr;
	}

	float	distance(Coords<T, dims> &toMesure){
		float out = 0;
		
		for (unsigned i = 0; i < dims; i++)
		{
			out += toMesure;
		}
	}

	T&		get(int i){
		return attr[i];
	}

	T&		operator[](int i){
		return this->get(i);
	}
};
	
}


#endif