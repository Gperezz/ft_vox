/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiwenn <maiwenn@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 15:57:37 by gperez            #+#    #+#             */
/*   Updated: 2021/06/17 17:33:34 by maiwenn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERROR_HPP__
# define __ERROR_HPP__

# include <iostream>
# include <exception>

# define FAIL_INIT_GLFW "Failed to initialize GLFW"
# define FAIL_INIT_GLAD "Failed to initialize GLAD"
# define FAIL_CREATE_WINDOW "Failed to create GLFW window"
# define JOINT_DONT_EXIST "The joint don't exist. Can't find this ID"

enum e_error {E_FAIL_INIT_GLFW, E_FAIL_INIT_GLAD, E_FAIL_CREATE_WINDOW, E_JOINT_DONT_EXIST};

class Error: public std::exception
{
	private:
		int					errNb;		//Numéro de l'erreur
		std::string			errStr;		//Description de l'erreur
		int					errLvl;		//Niveau de l'erreur

	public:
 		Error(int nb=0, std::string const& str="", int lvl=0) throw();
		virtual const char* what() const throw();
		int					getNb() const throw();
		int 				getLvl() const throw();
		virtual				~Error() throw();
};

#endif