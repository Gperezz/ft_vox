#include "Cave.hpp"
#include "Perlin.hpp"

typedef struct s_startPoint
{
	glm::vec3 noise;
	glm::vec3 chunk;
}				t_startPoint;

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

static void drawCave(glm::vec3 pos, Chunk *chunk, int x, int y, int z, unsigned char type)
{
	int positionX = (int)pos.x + x;
	int positionY = ((int)pos.y + y) % 16;
	int positionZ = (int)pos.z + z;
	if (positionX < 16 && positionX >= 0 && positionY < 256 && positionY >= 0 && positionZ < 16 && positionZ >= 0)
	{
		chunk->setBlock(BlockPos((int[4]){((int)pos.y + y) / 16, positionX, positionY, positionZ}),
			(t_block_info){type,0,0,0});
	}
}

static void dighing(glm::vec3 startInChunk, Chunk *chunk, glm::vec3 segment)
{
		for (int x = -2; x <= 2; x++){
			for(int y = -2; y <= 2; y++){
				for(int z = -2; z <= 2; z++){
					if (x * x + y * y + z * z <= 2 * 2 * 2){
						drawCave(startInChunk, chunk, x, y, z, AIR);
					}
				}
			}
		// }
	}
}

static glm::vec3 move(double angle)
{
	glm::vec3 segment = glm::vec3(4,-1,0);
	segment = rotateY(angle, segment);
	return(segment);
}

void startCave(Chunk *chunk, t_startPoint start, int size)
{
	double noises[size];
	PerlinNoise perlin = PerlinNoise(6);

	for (int i = 0; i < size; i++) {
		noises[i] = toDegree(perlin.noise(start.noise.x + i, start.noise.z + i));
	}
	glm::vec3 pos = start.chunk;
	for (int i = 0; i < size; i++) {
		glm::vec3 segment = move(noises[i]);
		dighing(pos, chunk, segment);
		pos += segment;
	}
}

static double elevation(double x, double z, double seed)
{
	PerlinNoise noise = PerlinNoise(seed);
	double e = (5.00 * noise.perlin(1, 0.005, 8, x, z) 
		+ 4.50 * noise.perlin(1, 0.001, 1, 7 * x, 2 * z)
		+ 3.25 * noise.perlin(1, 0.005, 2, 3 * x, 3 * z)
		+ 2.13 * noise.perlin(1, 0.004, 6, 4 * x, 4 * z)
		+ 1.06 * noise.perlin(1, 0.03, 1, 5 * x, 5 * z)
	);
    e = e / (5.00 + 4.50 + 3.25 + 2.13 + 1.06);
    e = pow(e, 1.0);
    return (e);
}


bool findCave(Chunk *chunk, int x, int z, t_startPoint *start)
{
	ChunkPos pos = chunk->getPos(); 

	if ((pos[0] + x) % 6 == 0 && (pos[1] + z)  % 6 == 0)
	{
		glm::vec3 blockPos = glm::vec3((pos[0] + x) % 16, 0, (pos[1] + z) % 16);
		start->noise = glm::vec3((pos[0] + x) * 16  + blockPos.x + (float)SHRT_MAX, 0, (pos[1] + z) * 16  + blockPos.z + (float)SHRT_MAX);
		
		double e = elevation(start->noise.x, start->noise.z, 1567612511);
		e = ((e + 1) / 2) * 255;
		start->chunk = glm::vec3(blockPos.x + (16 * x), e, blockPos.z + (16 * z));

		return(true);
	}
	return(false);
}

void Cave::createCave(Chunk *chunk)
{
	int size = 50;
	t_startPoint start;
	for (int i = -7; i < 7; i++)
	{
		for(int j = -7; j < 7; j++)
		{
			if (findCave(chunk, i, j, &start) == true)
				startCave(chunk, start, size);
		}
	}
}

Cave::~Cave()
{
}