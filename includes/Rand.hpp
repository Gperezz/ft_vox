/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rand.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 16:27:23 by gperez            #+#    #+#             */
/*   Updated: 2020/04/17 16:45:38 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAND_HPP
# define RAND_HPP

# include <ctime>
# include <math.h>
# include <functional>

template <typename T, typename Seed = unsigned long>
class Rand
{
	private:
		unsigned int nb;
		unsigned int a;
		unsigned int b;
		unsigned int m;
		std::hash<Seed> hasher;
	public:
		Rand()
		{
			a = 16807;
			b = 0;
			m = 0x7FFFFFFF;
			nb = time(NULL);
			generate();
		}
		Rand(Seed seed)
		{
			hasher = std::hash<Seed>();
			a = 16807;
			b = 0;
			m = 0x7FFFFFFF;
			nb = hasher(seed);
			generate();
		}

		T	generate()
		{
			nb = (a * nb + b) % m;
			return (nb);
		}

		T	generate(Seed new_seed){
			nb = hasher(new_seed);
			generate();
			return generate();
		}

		T	generate(T min, T max)
		{
			T		rdm;

			max -= min;
			rdm = roundf((float)generate() / (float)m * (float)max);
			return (rdm + min);
		}

		~Rand()
		{
		}
};
#endif