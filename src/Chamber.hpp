#pragma once
#include "Grid.hpp"
#include "Point.hpp"

class Chamber
{
public:
	const Grid<bool> walls_map;
	const Point start_pos;
	Chamber(const Grid<bool>& walls_map, const Point& start_pos);
	const bool is_wall(const Point& pos) const;
};
