#pragma once
#include "Grid.hpp"
#include "Point.hpp"
#include <list>
#include <set>

class Chamber
{
public:
	const Grid<bool> walls_map;
	const Point start_pos;
	const std::list<Point> way;
	Chamber(const Grid<bool>& walls_map, const Point& start_pos, const std::list<Point> way);
	const bool is_wall(const Point& pos) const;
	const unsigned int width() const;
	const unsigned int height() const;
};
