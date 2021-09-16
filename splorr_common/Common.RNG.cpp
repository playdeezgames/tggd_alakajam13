#include "Common.RNG.h"
#include <stdlib.h>
#include <time.h>
namespace common
{
	void RNG::Seed()
	{
		srand((unsigned int)time(nullptr));
	}

	int RNG::FromRange(int minimum, int maximum)
	{
		return rand() % (maximum - minimum) + minimum;
	}

	size_t RNG::FromRange(size_t minimum, size_t maximum)
	{
		return (size_t)FromRange((int)minimum, (int)maximum);
	}

	double RNG::FromRange(double minimum, double maximum)
	{
		return ((double)rand() / (double)RAND_MAX) * (maximum - minimum) + minimum;
	}
}