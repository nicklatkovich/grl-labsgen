#include "Chamber.hpp"

Chamber::Chamber(const Grid<bool>& walls_map, const Point& start_pos, const Point& exit_pos)
	: walls_map(walls_map)
	, start_pos(start_pos)
	, exit_pos(exit_pos)
	, key_poses({ start_pos, exit_pos })
{}

const bool Chamber::is_wall(const Point& pos) const
{
	return this->walls_map[pos];
}

const unsigned int Chamber::width() const
{
	return this->walls_map.width;
}

const unsigned int Chamber::height() const
{
	return this->walls_map.height;
}
