/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:48:55 by gperez            #+#    #+#             */
/*   Updated: 2020/03/30 18:06:51 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera()
{
	Mat();
	Camera::projection.setMatrix(glm::mat4(1.0f));
}

glm::mat4	Camera::getProjMatrix(void) const
{
	return (Camera::projection.getMatrix());
}

void		Camera::printProjectionMatrix(void)
{
		Camera::projection.printMatrix();
}

void		Camera::setProjMatrix(glm::mat4 mat)
{
	Camera::projection.setMatrix(mat);
}

Camera::~Camera()
{
}