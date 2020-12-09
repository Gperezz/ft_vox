/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karldouvenot <karldouvenot@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:29:17 by gperez            #+#    #+#             */
/*   Updated: 2020/11/28 16:03:45 by karldouveno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT_HPP
# define MAT_HPP
# include <mutex>

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
		Mat(Mat& copy);
		glm::mat4			getMatrix(void);
		glm::mat4			getMatrix(bool calc);
		void				setMatrix(glm::mat4 mat);
		void				printMatrix(bool calc);
		void				printMatrix(glm::mat4 mat);
		void				translate(glm::vec3 v);
		glm::vec3			getTranslate(void);
		void				setTranslate(glm::vec3 v);
		void				rotate(glm::vec3 v);
		void				setRotation(glm::vec3 rot);
		glm::vec3			getRotation(void);
		void				setScale(glm::vec3 v);
		void				scale(glm::vec3 s);
		glm::vec3			getScale(void);
		void				operator=(Mat& copy);
		virtual				~Mat();
	private:
		glm::mat4		matrix;
		glm::vec3		trans;
		glm::vec3		rot;
		glm::vec3		sc;

		virtual glm::mat4	calcMatrix(glm::vec3 t, glm::vec3 r, glm::vec3 s);
		virtual glm::mat4	calcMatrix(void);
	protected:
		std::mutex		translateMutex;
};

#endif