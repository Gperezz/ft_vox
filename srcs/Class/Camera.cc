/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:48:55 by gperez            #+#    #+#             */
/*   Updated: 2020/10/19 19:28:58 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera()
{
	Mat();
	this->projection.setMatrix(glm::mat4(1.0f));
	this->cameraFront = glm::vec3(0.0, 0.0, -1.0);
	this->cameraUp = glm::vec3(0.0, 1.0, 0.0);
}

glm::mat4	Camera::getProjMatrix(void)
{
	return (Camera::projection.getMatrix(false));
}

void		Camera::printProjectionMatrix(void)
{
		Camera::projection.printMatrix(false);
}

void		Camera::setProjMatrix(glm::mat4 mat)
{
	Camera::projection.setMatrix(mat);
}

void		Camera::lookAt(glm::vec3 look)
{
	this->setMatrix(glm::lookAt(this->getTranslate(), look, this->cameraUp));
}

void		Camera::look()
{
	this->setMatrix(glm::lookAt(this->getTranslate(), this->getTranslate() - cameraFront, this->cameraUp));
}

void		Camera::lookAt(void)
{
	this->lookAt(cameraFront);
}

void		Camera::translate(e_axes axe, float speed)
{
	if (axe == E_RIGHT)
		this->setTranslate(this->getTranslate() + glm::normalize(glm::cross(cameraFront, cameraUp)) * speed);
	else if (axe == E_UP)
		this->setTranslate(this->getTranslate() + cameraUp * speed);
	else
		this->setTranslate(this->getTranslate() + cameraFront * speed);
}

glm::mat4	Camera::calcMatrix(void)
{
	this->look();
	return (this->getMatrix());
}

Camera::~Camera()
{
}