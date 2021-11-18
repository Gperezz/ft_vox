#ifndef CAVE_HPP
# define CAVE_HPP

# include "Mat.hpp"

class PerlinWorms {
	private:
		glm::vec2 currentDirection;
		glm::vec2 currentPos;
		glm::vec2 convergancePoint;
		bool moveToCPoint;
	public:
		PerlinWorms(float seed, glm::vec2 pos);
		glm::vec2 PerlinWorms::getPerlinDirection();
		glm::vec2 PerlinWorms::move();
		~PerlinWorms();
};

#endif