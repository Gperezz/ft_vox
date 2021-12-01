#include "Perlin.hpp"

PerlinNoise::PerlinNoise()
{
	this->seed = random();
}

PerlinNoise::PerlinNoise(int s)
{
	this->seed = s;
}

double PerlinNoise::rand_noise(int t)
{
	t = (t << 13) ^ t;
	t = (t * (t * t * 13789 + 724321) + this->seed);
	return 1.0 - (t & 0x7fffffff) / 1073741824.0; 
}

double PerlinNoise::noise(int x, int y)
{
	int tmp = rand_noise(x) * 850000;
	return rand_noise(tmp + y);
}

double PerlinNoise::cubic_interpolate(double before, double p,
    double p2, double after, double t)
{
	double a3 = -0.5 * before + 1.5 * p - 1.5 * p2 + 0.5 * after;
	double a2 = before - 2.5 * p + 2 * p2 - 0.5 * after;
	double a1 = -0.5 * before + 0.5 * p2;
	double a0 = p;

	return (a3 * t * t * t) + (a2 * t * t) + (a1 * t) + a0;
}

double PerlinNoise::smooth_noise_firstdim(int integer_x,
	int integer_y, double fractional_x)
{
    double d0 = noise(integer_x - 1, integer_y);
    double d1 = noise(integer_x, integer_y);
    double d2 = noise(integer_x + 1, integer_y);
    double d3 = noise(integer_x + 2, integer_y);

    return cubic_interpolate(d0, d1, d2, d3, fractional_x);
}

double PerlinNoise::smooth_noise(double x, double y)
{
	int  integer_x;
	if(x >= 0)
		integer_x = (int)x;
	else
		integer_x = (int)x - 1;
	double fractional_x = x - integer_x;

	int integer_y = (int)y;
	double fractional_y = y - integer_y;

	double t0 = smooth_noise_firstdim(integer_x,
		integer_y - 1, fractional_x);
	double t1 = smooth_noise_firstdim(integer_x,
		integer_y, fractional_x);
	double t2 = smooth_noise_firstdim(integer_x,
		integer_y + 1, fractional_x);
	double t3 = smooth_noise_firstdim(integer_x,
		integer_y + 2, fractional_x);

	return cubic_interpolate(t0, t1, t2, t3, fractional_y);
}

double PerlinNoise::perlin(int octaves, double f, double persistence, double x, double y)
{
	double r = 0.;
	double amplitude = 1.;
	double redis;

	for(int i = 0; i < octaves; i++)
	{
		r += smooth_noise(x * f + i, y * f + i) * amplitude;
		amplitude *= persistence;
		f *= 2;
	}

	if (persistence == 1.)
		redis = 1;
	else
		redis = (1 - persistence) / (1 - amplitude);
	r = r / 2;
	return (r) * redis;
}

PerlinNoise::~PerlinNoise()
{
}