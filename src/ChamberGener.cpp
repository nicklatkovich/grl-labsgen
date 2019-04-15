#include "ChamberGener.hpp"
#include "Cell.hpp"
#include "Grid.hpp"
#include "Point.hpp"
#include "UnorderedArray.hpp"
#include <cstdlib>
#include <tuple>

Point DD[4] = { Point(1, 0), Point(0, -1), Point(-1, 0), Point(0, 1) };

ChamberGener::ChamberGener(unsigned int width, unsigned int height, unsigned int seed)
	: width(width)
	, height(height)
	, seed(seed)
{}

template<class T>
Point get_random_pos(const Grid<T>& grid)
{
	Point result;
	result.x = (unsigned int)rand() % (grid.width - 2) + 1;
	result.y = (unsigned int)rand() % (grid.height - 2) + 1;
	return result;
}

Chamber ChamberGener::run() const
{
	srand(this->seed);
	Grid<gl::CELL> grid(this->width, this->height, gl::CELL::EMPTY);
	grid.set_borders(gl::CELL::CLOSED);
	Point start_pos = get_random_pos(grid);
	grid[start_pos] = gl::CELL::POINT;
	UnorderedArray<Point> points;
	points.push(start_pos);
	while (points.get_length() > 0) {
		Point pos = points.pop();
		for (Point d : DD) {
			gl::CELL prev_cell = grid[pos];
			UnorderedArray<unsigned int> possible_steps_counts;
			for (auto [steps_count, possible_closed_cell_pos] = std::tuple<unsigned int, Point>{ 1u, pos };;
				 steps_count++) {
				possible_closed_cell_pos += d;
				gl::CELL cell = grid[possible_closed_cell_pos];
				if (prev_cell != gl::CELL::POINT && (cell == gl::CELL::EMPTY || cell == gl::CELL::CLOSED)) {
					possible_steps_counts.push(steps_count);
				}
				if (grid[possible_closed_cell_pos] == gl::CELL::CLOSED) {
					break;
				}
				prev_cell = cell;
			}
			if (possible_steps_counts.get_length() == 0) {
				continue;
			}
			unsigned int steps_count = possible_steps_counts.pop();
			Point pos_to_change = pos + d;
			for (unsigned int step_number = 2; step_number < steps_count;
				 step_number++, pos_to_change = pos_to_change + d) {
				if (grid[pos_to_change] == gl::CELL::EMPTY) {
					grid[pos_to_change] = gl::CELL::FLOW;
				}
			}
			grid[pos_to_change] = gl::CELL::POINT;
			points.push(pos_to_change);
			grid[pos_to_change + d] = gl::CELL::CLOSED;
		}
	}
	std::function<bool(gl::CELL)> map_function = [](gl::CELL cell) -> bool {
		return cell == gl::CELL::CLOSED || cell == gl::CELL::EMPTY;
	};
	return Chamber(grid.map(map_function), start_pos);
}
