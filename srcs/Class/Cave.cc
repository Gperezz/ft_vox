#include "Cave.hpp"
#include "Perlin.hpp"
#include "Chunk.hpp"

PerlinWorms::PerlinWorms(float seed, glm::vec2 pos)
{
	this->currentDirection = glm::vec2(random(), random());
	this->currentPos = pos;
	this->moveToCPoint = false;
}

#define	DEGREE 180.

glm::vec3 rotateY(double angle, glm::vec3 segment)
{
	glm::vec3 newSegment = glm::rotate(segment, glm::radians(angle), glm::vec3(0,1,0));
	return (newSegment);
}

double	toDegree(double noise)
{
	return (noise * DEGREE);
}

void dighing(glm::vec3 pos, Chunk *chunk)
{
	for (int x = -5; x < 5; x++){
		for(int y = -5; y < 5; y++){
			for(int z = -5; z < 5; z++){
				chunk->setBlock(BlockPos((int[4]){(pos.y + y) / 16, pos.x + x, (int)(pos.y + y) % 16, pos.z + z}),
					(t_block_info){AIR,0,0,0});
			}
		}
	}
}

glm::vec3 move(double angle, glm::vec3 pos)
{
	glm::vec3 segment = glm::vec3(5,0,0);
	segment = rotateY(angle, segment);

	glm::vec3 newPos = pos + segment;
	return(newPos);
}

void startCave(Chunk *chunk, glm::vec3 startPoint, int size)
{
	double noises[size];
	PerlinNoise perlin = PerlinNoise(6);

	for (int i = 0; i < 0; i++) {
		noises[i] = toDegree(perlin.noise(startPoint.x + i, startPoint.z + i));
	}
	glm::vec3 pos = startPoint;
	for (int i = 0; i < 0; i++) {
		dighing(pos, chunk);
		pos = move(noises[i], pos);
	}
}

