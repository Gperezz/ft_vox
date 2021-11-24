#ifndef CAVE_HPP
# define CAVE_HPP

# include "Mat.hpp"
# include "Chunk.hpp"

class Cave {
	private:
		
	public:
		Cave();
		void createCave(Chunk *chunk);
		~Cave();
};

#endif