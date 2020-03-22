/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:35:15 by gperez            #+#    #+#             */
/*   Updated: 2020/03/22 19:53:53 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ENGINE_HPP
# define _ENGINE_HPP

# include "World.hpp"

class Engine
{
	private:
		GLFWwindow	*window;
		World		world;
	public:
		Engine();
		GLFWwindow	*getWindow(void);
		~Engine();
};

#endif