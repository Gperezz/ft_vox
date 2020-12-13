/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hud.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperez <gperez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 20:11:46 by gperez            #+#    #+#             */
/*   Updated: 2020/12/13 00:08:39 by gperez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _HUD_HPP_
# define _HUD_HPP_

# include "Element.hpp"
# include "Textures.hpp"
# include "Shader.hpp"

# include <vector>
# include "glm.hpp" // Implementation matrices

# define SQUARE_PTS 4
# define VERTEX_HUD "shader/vertexHud.glsl"
# define FRAGMENT_HUD "shader/fragmentHud.glsl"

static float	square_pt[] = {
	-1.0, 1.0,
	-1.0, -1.0,
	1.0, 1.0,
	1.0, -1.0
};

class Hud
{
	private:
		std::vector<Element*>	hudElements;
		Shader					shader;
		glm::mat4				projection;
		bool					isInit;
		void					genTexture(void);
		void					fillElements(std::vector<char> unitsValue, unsigned int units);
		void					syncFpsToElements(int fps);
	public:
		Hud();
		void					setProjection(glm::mat4 proj);
		glm::mat4				getProjection(void);
		int						init(glm::mat4 proj);
		void					newElement(void);
		void					display(int fps, unsigned int txt);
		void					deleteElement(unsigned int i);
		~Hud();
};

#endif