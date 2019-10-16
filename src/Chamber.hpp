#pragma once
#include "Grid.hpp"
#include "Point.hpp"

class Chamber
{
public:
	const Grid<bool> walls_map;
	const Point start_pos;
	const Point exit_pos;
	Chamber(const Grid<bool>& walls_map, const Point& start_pos, const Point& exit_pos);
	const bool is_wall(const Point& pos) const;
	const unsigned int width() const;
	const unsigned int height() const;
};
