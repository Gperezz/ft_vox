#ifndef CAVE_HPP
# define CAVE_HPP

# include "Mat.hpp"
# include "Chunk.hpp"

class Cave {
	private:
		
	public:
		Cave();
		void startCave(Chunk *chunk, glm::vec2 chunkPos, glm::vec3 startPoint, int size);
		void createCave(Chunk *chunk);
		~Cave();
};

#endif