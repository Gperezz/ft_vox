/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Coords.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 20:07:59 by gperez            #+#    #+#             */
/*   Updated: 2020/04/13 20:18:18 by gperez           ###   ########.fr       */
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
	Coords(){
		bzero(attr, sizeof(T) * dims);
	}
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

	T&		get(unsigned i){
		return attr[i];
	}

	T&		operator[](unsigned i){
		return this->get(i);
	}
	Coords<T, dims>	*add(Coords<T, dims> a)
	{
		for (unsigned i = 0; i < dims; i++)
		{
			this->attr[i] += a.get(i);
		}
		return this;
	}
	Coords<T, dims> *operator+(Coords<T, dims> a)
	{
		return this->add(add);
	}
};
	
}


#endif