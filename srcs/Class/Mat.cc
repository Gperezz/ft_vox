/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:19:34 by gperez            #+#    #+#             */
/*   Updated: 2020/10/20 03:40:53 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mat.hpp"

Mat::Mat()
{
	Mat::matrix = glm::mat4(1.0f);
	Mat::trans = glm::vec3(0.0, 0.0, 0.0);
	Mat::rot = glm::vec3(0.0, 0.0, 0.0);
	Mat::sc = glm::vec3(1.0, 1.0, 1.0);
}

glm::mat4	Mat::getMatrix(void)
{
	return (this->matrix);
}

glm::mat4	Mat::getMatrix(bool calc)
{
	if (calc)
		this->calcMatrix();
	return (this->matrix);
}

glm::mat4	Mat::calcMatrix(glm::vec3 t, glm::vec3 r, glm::vec3 s)
{
	glm::mat4	matrix;

	matrix = glm::translate(glm::mat4(1.0f), t);

	matrix = glm::rotate(matrix, glm::radians(r.x), glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(r.y), glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, glm::radians(r.z), glm::vec3(0.0, 0.0, 1.0));

	matrix = glm::scale(matrix, s);

	return (matrix);
}

glm::mat4	Mat::calcMatrix(void)
{
	this->matrix = this->calcMatrix(this->trans, this->rot, this->sc);
	return (this->matrix);
}

void		Mat::setMatrix(glm::mat4 mat)
{
	Mat::matrix = mat;
}

void		Mat::printMatrix(glm::mat4 mat)
{
	ft_putchar('\n');
	ft_printf("|{red}%f {green}%f {bold_blue}%f {na}%f|\n",
		mat[0][0],
		mat[1][0],
		mat[2][0],
		mat[3][0]);
	ft_printf("|{red}%f {green}%f {bold_blue}%f {na}%f|\n",
		mat[0][1],
		mat[1][1],
		mat[2][1],
		mat[3][1]);
	ft_printf("|{red}%f {green}%f {bold_blue}%f {na}%f|\n",
		mat[0][2],
		mat[1][2],
		mat[2][2],
		mat[3][2]);
	ft_printf("|{red}%f {green}%f {bold_blue}%f {na}%f|\n",
		mat[0][3],
		mat[1][3],
		mat[2][3],
		mat[3][3]);
	ft_putchar('\n');
}

void		Mat::printMatrix(bool calc)
{
	if (calc)
		this->calcMatrix();
	this->printMatrix(this->matrix);
}

void		Mat::translate(glm::vec3 v)
{
	Mat::trans += v;
}

void		Mat::setTranslate(glm::vec3 v)
{
	Mat::trans = v;
}

glm::vec3	Mat::getTranslate(void)
{
	return (Mat::trans);
}

void		Mat::rotate(glm::vec3 v)
{
	Mat::rot += v;
}

void	Mat::setRotation(glm::vec3 v)
{
	this->rot = v;
}

glm::vec3	Mat::getRotation(void)
{
	return (Mat::rot);
}

void		Mat::setScale(glm::vec3 v)
{
	Mat::sc = v;
}

void		Mat::scale(glm::vec3 s)
{
	Mat::sc += s;
}

glm::vec3	Mat::getScale(void)
{
	return (Mat::sc);
}

Mat::~Mat()
{
}
