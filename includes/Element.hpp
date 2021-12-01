/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Element.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 21:33:58 by gperez            #+#    #+#             */
/*   Updated: 2021/11/27 23:59:54 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ELEMENT_HPP_
# define _ELEMENT_HPP_

extern "C"
{
	#include "glad/glad.h" // Implementation OpenGl
}

# include "Mat.hpp"

class Element : public Mat
{
	private:
		unsigned int	vbo;
		unsigned int	vao;
		char			offsetTxt;
	public:
		Element();
		void			setVbo(unsigned int v);
		unsigned int	getVbo(void);
		void			setVao(unsigned int v);
		unsigned int	getVao(void);
		void			setOffsetTxt(char offset);
		char			getOffsetTxt(void);
		~Element();
};

#endif