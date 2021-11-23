#include "Cave.hpp"
#include "Perlin.hpp"

Cave::Cave()
{
}

#define	DEGREE 180.

static glm::vec3 rotateY(double angle, glm::vec3 segment)
{
	const double cos_a = cos(angle);
	const double sin_a = sin(angle);

	return (glm::vec3(
		segment.x * cos_a + segment.z * sin_a,
		segment.y,
		-segment.x * sin_a + segment.z * cos_a
	));
}

static double	toDegree(double noise)
{
	return (noise * DEGREE);
}

static void drawCave(glm::vec3 pos, glm::vec2 chunkPos, Chunk *chunk, int x, int y, int z)
{
	int positionX = (int)pos.x + x + (chunkPos.x * 16);
	int positionY = ((int)pos.y + y) % 16;
	int positionZ = (int)pos.z + z + (chunkPos.y * 16);
	if (positionX < 16 && positionX >= 0 && positionY < 256 && positionY >= 0 && positionZ < 16 && positionZ >= 0)
	{
		chunk->setBlock(BlockPos((int[4]){((int)pos.y + y) / 16, positionX, positionY, positionZ}),
			(t_block_info){DIRT,0,0,0});
	}
}

static void dighing(glm::vec3 pos, glm::vec2 chunkPos, Chunk *chunk)
{
	// printf("-------pos: %f, %f, %f, \n", pos.x, pos.y, pos.z);
	for (int x = -2; x <= 2; x++){
		for(int y = -2; y <= 2; y++){
			for(int z = -2; z <= 2; z++){
				if (x * x + y * y + z * z <= 2 * 2 * 2){
					drawCave(pos, chunkPos, chunk, x, y, z);
				}
			}
		}
	}
}

static glm::vec3 move(double angle, glm::vec3 pos)
{
	// printf("------Move, angle: %f\n", angle);
	glm::vec3 segment = glm::vec3(3,-3,0);
	segment = rotateY(angle, segment);

	glm::vec3 newPos = pos + segment;
	return(newPos);
}

void Cave::startCave(Chunk *chunk, glm::vec2 chunkPos, glm::vec3 startPoint, int size)
{
	// printf("start\n");
	double noises[size];
	PerlinNoise perlin = PerlinNoise(6);

	for (int i = 0; i < size; i++) {
		noises[i] = toDegree(perlin.noise(startPoint.x + i, startPoint.z + i));
		// printf("--noise\n");
	}
	glm::vec3 pos = startPoint;
	for (int i = 0; i < size; i++) {
		// printf("----dighing\n");
		dighing(pos, chunkPos, chunk);
		pos = move(noises[i], pos);
		// pos.y += 5;
	}
	// printf("out\n\n\n");
}

bool findCave(Chunk *chunk, int x, int z, glm::vec3 *startPoint)
{
	ChunkPos pos = chunk->getPos();

	if ((pos[0] + x) % 3 == 0 && (pos[1] + z)  % 3 == 0)
	{
		*startPoint = glm::vec3(pos[0] + x, 200, pos[1] + z);
		return(true);
	}
	return(false);
}

void Cave::createCave(Chunk *chunk)
{
	int size = 40;
	glm::vec3 startPoint;
	for (int i = -3; i < 3; i++)
	{
		for(int j = -3; j < 3; j++)
		{
			if (findCave(chunk, i, j, &startPoint) == true)
				startCave(chunk, glm::vec2(i,j), startPoint, size);
		}
	}
	

}
//if cave creuve
//si sur mon chunk ty a une cave 

Cave::~Cave()
{
}