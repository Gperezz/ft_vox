/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:49:41 by gperez            #+#    #+#             */
/*   Updated: 2020/04/22 19:44:24 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

#include "Mat.hpp"

class	Camera : public Mat
{
	public:
		Camera();
		glm::mat4	getProjMatrix(void) const;
		void		printProjectionMatrix(void);
		void		setProjMatrix(glm::mat4 mat);
		~Camera();
	private:
		Mat		projection;
};

#endif
