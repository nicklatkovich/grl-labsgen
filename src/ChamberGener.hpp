#pragma once
#include "Chamber.hpp"

class ChamberGener {
public:
	unsigned int width, height, seed;
	ChamberGener(
		unsigned int width,
		unsigned int height,
		unsigned int seed = 0);
	Chamber run() const;
};
