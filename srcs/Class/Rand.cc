#include "Rand.hpp"

Rand::Rand()
{
	a = 16807;
	b = 0;
	m = 0x7FFFFFFF;
	nb = time(NULL);
	generate();
}
Rand::Rand(Seed seed)
{
	hasher = std::hash<Seed>();
	a = 16807;
	b = 0;
	m = 0x7FFFFFFF;
	nb = hasher(seed);
	generate();
}

T	Rand::generate()
{
	nb = (a * nb + b) % m;
	return (nb);
}

T	Rand::generate(Seed new_seed){
	nb = hasher(new_seed);
	generate();
	return generate();
}

T	Rand::generate(T min, T max)
{
	T		rdm;

	max -= min;
	rdm = roundf((float)generate() / (float)m * (float)max);
	return (rdm + min);
}

~Rand::Rand()
{
}