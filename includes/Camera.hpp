/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 23:49:41 by gperez            #+#    #+#             */
/*   Updated: 2020/10/20 16:10:18 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "Mat.hpp"
# define SPEED 2.5f
# define SENSITIVITY 0.15f

enum	e_axes{E_RIGHT, E_UP, E_FRONT};
enum	e_rot{PITCH, YAW, ROLL};

class	Camera : public Mat
{
	public:
		Camera();
		glm::mat4			getProjMatrix(void);
		void				printProjectionMatrix(void);
		void				setProjMatrix(glm::mat4 mat);
		void				lookAt(glm::vec3 look);
		void				lookAt(void);
		void				look(void);
		virtual void		translate(e_axes axe, float speed);
		virtual void		rotate(glm::vec3 rotEuler);
		float				getEuler(e_rot euler);
		void				setCameraFront(glm::vec3 front);
		virtual				~Camera();
	private:
		Mat					projection;
		glm::vec3			cameraFront;
		glm::vec3			cameraUp;
		virtual glm::mat4	calcMatrix(void);
};

#endif
