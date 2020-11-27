/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:48:55 by gperez            #+#    #+#             */
/*   Updated: 2020/10/20 16:10:22 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera()
{
	Mat();
	this->projection.setMatrix(glm::mat4(1.0f));
	this->cameraUp = glm::vec3(0.0, 1.0, 0.0);
	this->setRotation((glm::vec3){0.0, -90.0, 0.0});
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

void		Camera::look(void)
{
	this->setCameraFront(glm::normalize(
		glm::vec3(cos(glm::radians(this->getEuler(YAW))) * cos(glm::radians(this->getEuler(PITCH))),
			sin(glm::radians(this->getEuler(PITCH))),
				sin(glm::radians(this->getEuler(YAW))) * cos(glm::radians(this->getEuler(PITCH))))));
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

void	Camera::rotate(glm::vec3 rotEuler)
{
	Mat::rotate(rotEuler);
	if (this->getRotation().x > 89.9)
		this->setRotation(glm::vec3(89.9, this->getRotation().y, this->getRotation().z));
	else if (this->getRotation().x < -89.9)
		this->setRotation(glm::vec3(-89.9, this->getRotation().y, this->getRotation().z));
}

float	Camera::getEuler(e_rot euler)
{
	if (euler == PITCH)
		return (this->getRotation().x);
	else if (euler == YAW)
		return (this->getRotation().y);
	else
		return (this->getRotation().z);
}

void	Camera::setCameraFront(glm::vec3 front)
{
	this->cameraFront = front;
}

glm::mat4	Camera::calcMatrix(void)
{
	this->look();
	return (this->getMatrix());
}

Camera::~Camera()
{
}