/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:29:17 by gperez            #+#    #+#             */
/*   Updated: 2020/05/05 12:39:02 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT_HPP
# define MAT_HPP

extern "C"
{
	# include "ft_printf.h"
}
# include "glm.hpp"
# include "matrix_transform.hpp"
# include "type_ptr.hpp"

class					Mat
{
	public:
		Mat();
		glm::mat4			getMatrix(bool calc);
		void				setMatrix(glm::mat4 mat);
		void				printMatrix(bool calc);
		void				printMatrix(glm::mat4 mat);
		void				translate(glm::vec3 v);
		glm::vec3			getTranslate(void);
		void				setTranslate(glm::vec3 v);
		void				rotate(glm::vec3 v);
		glm::vec3			getRotation(void);
		void				setScale(glm::vec3 v);
		void				scale(glm::vec3 s);
		glm::vec3			getScale(void);
		~Mat();
	private:
		glm::mat4		matrix;
		glm::vec3		trans;
		glm::vec3		rot;
		glm::vec3		sc;

		glm::mat4			calcMatrix(glm::vec3 t, glm::vec3 r, glm::vec3 s);
		glm::mat4			calcMatrix(void);

};

#endif