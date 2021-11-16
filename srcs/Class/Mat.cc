/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat.cc                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:19:34 by gperez            #+#    #+#             */
/*   Updated: 2021/11/11 19:37:49 by gperez           ###   ########.fr       */
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

Mat::Mat(Mat& copy)
:
	matrix(copy.matrix),
	trans(copy.trans),
	rot(copy.rot),
	sc(copy.sc),
	translateMutex()
{}

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
	printf("%f %f %f %f|\n",
		mat[0][0],
		mat[1][0],
		mat[2][0],
		mat[3][0]);
	printf("%f %f %f %f|\n",
		mat[0][1],
		mat[1][1],
		mat[2][1],
		mat[3][1]);
	printf("%f %f %f %f|\n",
		mat[0][2],
		mat[1][2],
		mat[2][2],
		mat[3][2]);
	printf("%f %f %f %f|\n",
		mat[0][3],
		mat[1][3],
		mat[2][3],
		mat[3][3]);
	std::cout << '\n';
}

void		Mat::printMatrix(bool calc)
{
	if (calc)
		this->calcMatrix();
	this->printMatrix(this->matrix);
}

void		Mat::translate(glm::vec3 v)
{
	// std::unique_lock<std::mutex> lk(this->translateMutex);
	Mat::trans += v;
}

void		Mat::setTranslate(glm::vec3 v)
{
	// std::unique_lock<std::mutex> lk(this->translateMutex);
	Mat::trans = v;
}

glm::vec3	Mat::getTranslate(void)
{
	glm::vec3 out;
	// std::unique_lock<std::mutex> lk(this->translateMutex);
	out = Mat::trans;
	return (out);
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

void	Mat::operator=(Mat& copy)
{
	this->matrix = copy.matrix;
	this->trans = copy.trans;
	this->rot = copy.rot;
	this->sc = copy.sc;

	return ;
}

Mat::~Mat()
{
}
