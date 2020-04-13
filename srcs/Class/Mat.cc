/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:19:34 by gperez            #+#    #+#             */
/*   Updated: 2020/03/30 18:05:57 by gperez           ###   ########.fr       */
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

glm::mat4	Mat::getMatrix(void) const
{
	return (Mat::matrix);
}

glm::mat4	Mat::calcMatrix(void)
{
	Mat::matrix = glm::translate(glm::mat4(1.0f), Mat::trans);

	Mat::matrix = glm::rotate(Mat::matrix, glm::radians(Mat::rot.x), glm::vec3(1.0, 0.0, 0.0));
	Mat::matrix = glm::rotate(Mat::matrix, glm::radians(Mat::rot.y), glm::vec3(0.0, 1.0, 0.0));
	Mat::matrix = glm::rotate(Mat::matrix, glm::radians(Mat::rot.z), glm::vec3(0.0, 0.0, 1.0));

	Mat::matrix = glm::scale(Mat::matrix, Mat::sc);

	return (Mat::matrix);
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

void		Mat::printMatrix(void)
{
	Mat::printMatrix(Mat::matrix);
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
