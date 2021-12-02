#ifndef PERLIN_HPP
# define PERLIN_HPP

#include <map>
#include <random>
#include <cmath>
using namespace std;

class PerlinNoise {
	private:
		int seed;
	public:
		PerlinNoise(int s);
		double	perlin(int octaves, double frequency, double persistence, double x, double y);
		double cubic_interpolate(double before, double p, double p2, double after, double t);
		double smooth_noise_firstdim(int integer_x, int integer_y, double fractional_x);
		double smooth_noise(double x, double y);
		double rand_noise(int t);
		double noise(int x, int y);
		~PerlinNoise();
};

#endif