/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vox.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 17:43:41 by gperez            #+#    #+#             */
/*   Updated: 2020/03/21 20:27:42 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_VOX_HPP_
# define _FT_VOX_HPP_

extern "C"
{
	# include "ft_printf.h"
	# include "glad.h" // Implementation OpenGl
}

# include "glfw3.h" // Load fenetre
# include "stb_image.h" // Load image
# include "glm.hpp" // Implementation matrices

# include "Chunck.hpp"
# include "Block.hpp"

# define WIDTH 800
# define HEIGHT 600

#endif