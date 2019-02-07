#include "Chamber.hpp"

Chamber::Chamber(const Grid<bool>& walls_map, const Point& pos)
	: walls_map(walls_map)
	, start_pos(Point(pos))
{
}

const bool Chamber::is_wall(const Point& pos) const
{
	return this->walls_map[pos];
}
