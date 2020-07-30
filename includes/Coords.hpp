/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Coords.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 20:07:59 by gperez            #+#    #+#             */
/*   Updated: 2020/04/20 02:13:53 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _COORDS_HPP_
# define _COORDS_HPP_

# include <cstring>
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
					out += toMesure;
				}
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
				for (int i = 0; i < dims; i++)
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
		class hash<Coords<T, dims>>{
			size_t operator()(const Coords<T, dims> &x) const{
				size_t out = hash<T>()(x.attr[0]);
				for (int i = 1; i < dims; i++)
				{
					out ^= hash<T>()(x.attr[i]);
				}
				return out;
			}
		}
}
#endif