#include "representers.hpp"
#include <stdexcept>

const char grlrepresenters::coord2char(const unsigned int coord)
{
	if (coord < 10) {
		return '0' + coord;
	}
	if (coord < 'Z' - 'A' + 11) {
		return 'A' + coord - 10;
	}
	throw std::runtime_error("unable to present coord");
}
