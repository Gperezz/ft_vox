/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:35:15 by gperez            #+#    #+#             */
/*   Updated: 2020/03/25 19:04:28 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ENGINE_HPP
# define _ENGINE_HPP

# include "World.hpp"
# include "Shader.hpp"
# include <iostream>

# define WIDTH 800
# define HEIGHT 600

class Engine
{
	private:
		GLFWwindow	*window;
		World		world;
		Shader		shader;
	public:
		Engine();
		int			initWindow(void);
		GLFWwindow	*getWindow(void);
		Shader		getShader(void);
		~Engine();
};

#endif