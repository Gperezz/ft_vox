/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rand.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 16:27:23 by gperez            #+#    #+#             */
/*   Updated: 2020/04/16 07:52:38 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAND_HPP
# define RAND_HPP

# include <ctime>
template <typename T>
class Rand
{
	private:
		unsigned int nb;
		unsigned int a;
		unsigned int b;
		unsigned int m;
	public:
		Rand::Rand()
		{
			Rand::a = 16807;
			Rand::b = 0;
			Rand::m = 0x7FFFFFFF;
			Rand::nb = time(NULL);
			Rand::generate();
		}
		Rand::Rand(unsigned long seed)
		{
			Rand::a = 16807;
			Rand::b = 0;
			Rand::m = 0x7FFFFFFF;
			Rand::nb = seed;
			Rand::generate();
		}

		T	Rand::generate()
		{
			Rand::nb = (Rand::a * Rand::nb + Rand::b) % Rand::m;
			return (Rand::nb);
		}

		T		Rand::generate(T min, T max)
		{
			T		rdm;

			max = max - min;
			rdm = roundf((float)Rand::generate() / (float)Rand::m * (float)max);
			return (rdm + min);
		}

		Rand::~Rand()
		{
		}
};
#endif