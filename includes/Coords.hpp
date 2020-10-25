/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Coords.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 20:07:59 by gperez            #+#    #+#             */
/*   Updated: 2020/10/25 23:25:27 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COORDS_HPP_
# define _COORDS_HPP_

# include <cstring>
# include <functional>
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
				memcpy(this->attr, attr, sizeof(T) * dims);
			}

			Coords(const Coords<T, dims> &toClone){
				memcpy(this->attr, toClone.attr, sizeof(T) * dims);
			}

			float		distance(Coords<T, dims> &toMesure){
				float out = 0;
				
				for (unsigned i = 0; i < dims; i++)
				{
					out += (this->get(i) - toMesure.get(i)) * (this->get(i) - toMesure.get(i));
				}
				return sqrt(out);
			}

			T&			get(const unsigned i)
			{
				return (this->attr[i]);
			}
			
			const T&	getConst(const unsigned i) const
			{
				return (this->attr[i]);
			}

			T&			operator[](const unsigned i)
			{
				return this->get(i);
			}
			Coords<T, dims> add(Coords<T, dims> a)
			{
				Coords<T, dims> out(*this);
				for (unsigned i = 0; i < dims; i++)
				{
					out[i] += a.get(i);
				}
				return out;
			}
			inline		Coords<T, dims> operator+(Coords<T, dims> a)
			{
				return (this->add(a));
			}

			inline bool	operator<(const Coords<T, dims>& a) const
			{
				for (unsigned int i = 0; i < dims; i++)
				{
					if (this->getConst(i) != a.getConst(i))
						return this->getConst(i) < a.getConst(i);
				}
				return false;
			}

			inline bool	equals(const Coords<T, dims>& a) const
			{
				for (unsigned i = 0; i < dims; i++)
				{
					if (this->getConst(i) != a.getConst(i))
						return false;
				}
				return true;
			}

			inline bool	operator==(const Coords<T, dims>& a) const
			{
				return (equals(a));
			}

			inline bool	operator!=(const Coords<T, dims>& a) const
			{
				return (!equals(a));
			}
	};
	
}

namespace std{
	template<typename T, unsigned dims>
	struct hash<Coords::Coords<T, dims>>{
		size_t operator()(Coords::Coords<T, dims> const &x) const{
			size_t out = hash<T>()(x.getConst(0));
			for (unsigned i = 1; i < dims; i++)
				out ^= hash<T>()(x.getConst(i));
			return out;
		}
	};
};
#endif