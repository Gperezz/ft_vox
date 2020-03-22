/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cc                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 19:52:39 by gperez            #+#    #+#             */
/*   Updated: 2020/03/22 19:54:35 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

Engine::Engine()
{
}

GLFWwindow	*Engine::getWindow(void)
{
	return (Engine::window);
}

Engine::~Engine()
{
}