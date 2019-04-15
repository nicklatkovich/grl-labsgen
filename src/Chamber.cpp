#include "Chamber.hpp"

Chamber::Chamber(const Grid<bool>& walls_map, const Point& pos, const Point& exit_pos)
	: walls_map(walls_map)
	, start_pos(pos)
	, exit_pos(exit_pos)
{}

const bool Chamber::is_wall(const Point& pos) const
{
	return this->walls_map[pos];
}
