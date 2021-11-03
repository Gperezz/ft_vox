#include <map>
#include <random>
#include <cmath>
using namespace std;

struct perlinSeed {
	int seed;
	int layer;
	int x;
	int z;

	bool operator<(const perlinSeed& a) const{
		if (this->seed == a.seed) {
			if (this->layer == a.layer) {
				if (this->x == a.x) {
					return this->z < a.z;
				}
				return this->x < a.x;
			}
			return this->layer < a.layer;
		}
		return this->seed < a.seed;
	}
};

struct vec2 {
	float x;
	float z;

	vec2(float ax, float az, float bx, float bz){
		this->x = bx - ax;
		this->z = bz - az;
	}

	vec2(float x, float z){
		this->x = x;
		this->z = z;
	}

	vec2(){
		this->x = 0;
		this->z = 0;
	}

	vec2(const vec2& from){
		this->x = from.x;
		this->z = from.z;
	}
	vec2(const vec2&& from){
		this->x = from.x;
		this->z = from.z;
	}
	void operator=(const vec2& from){
		this->x = from.x;
		this->z = from.z;
	}

	float dotProduct(vec2 vec) {
		return vec.x * this->x + vec.z * this->z;
	}
};

class PerlinNoise {
private:
	int cellSize;
	int seed;
	map<perlinSeed, vec2> dotCells;
public:
	PerlinNoise(){}
	PerlinNoise(int cellSize, long seed)
	{
		this->cellSize = cellSize;
		this->seed = seed;
		this->dotCells = map<perlinSeed, vec2>();
	}

	vec2 randomGradient(perlinSeed key)
	{
		uniform_real_distribution<float> distr(-1.0, 1.0);
		seed_seq seq = {key.seed, key.layer, key.x, key.z};
		minstd_rand0 generator = minstd_rand0(seq);
		return {distr(generator), distr(generator)};
	}

	vec2 gradient(int layer, int x, int z)
	{
		perlinSeed key = {this->seed, layer, x, z};
		if (this->dotCells.count(key) != 0)
			return this->dotCells.at(key);
		vec2 vec = randomGradient(key);
		this->dotCells[key] = vec;
		return vec;
	}

	float dotGridGradient(float w, float xi, float yi, float x, float y) {
		vec2 vec = vec2(xi, yi, x, y);

		return vec.dotProduct(this->gradient(w, xi, yi));
	}

	float lerp(float a0, float a1, float w)
	{
		return (1.0 - w) * a0 + w * a1;
	}

	float cosint(float a, float b, float t)
	{
		float c = (1 - cos(t * 3.1415927)) * 0.5;

		return (1 - c) * a + c * b;
	}

	float perlin(float w, float x, float y) {
		// Determine grid cell coordinates
		x /= this->cellSize;
		y /= this->cellSize;

		float x0 = (int)x;
		float x1 = x0 + 1;
		float y0 = (int)y;
		float y1 = y0 + 1;
		// console.log(x0, y);
		float sx = x - x0;
		float sy = y - y0;

		float n0, n1, ix0, ix1, value;
		n0 = this->dotGridGradient(w, x0, y0, x, y);
		n1 = this->dotGridGradient(w, x1, y0, x, y);
		// console.log(n0, n1);

		ix0 = this->cosint(n0, n1, sx);

		n0 = this->dotGridGradient(w, x0, y1, x, y);
		n1 = this->dotGridGradient(w, x1, y1, x, y);
		// console.log(n0, n1);

		ix1 = this->cosint(n0, n1, sx);
		value = this->cosint(ix0, ix1, sy);
		// console.log(ix0, ix1, value);

		return value;
	}

	float operator()(float w, float x, float y){
		return this->perlin(w, x, y);
	}
};