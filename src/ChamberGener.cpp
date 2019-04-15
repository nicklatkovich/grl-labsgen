#include "ChamberGener.hpp"
#include "Cell.hpp"
#include "Grid.hpp"
#include "Point.hpp"
#include "UnorderedArray.hpp"
#include <cstdlib>
#include <queue>
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

void process_point(UnorderedArray<Point>& points, Grid<gl::CELL>& grid)
{
	Point pos = points.pop();
	for (Point d : DD) {
		gl::CELL prev_cell = grid[pos];
		UnorderedArray<unsigned int> possible_steps_counts;
		Point possible_closed_cell_pos = pos;
		for (unsigned int steps_count = 1;; steps_count++) {
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

Point get_exit_pos(const Grid<bool>& walls_map, const Point& start_pos)
{
	Grid<unsigned int> steps(walls_map.width, walls_map.height, UINT32_MAX);
	steps[start_pos] = 0;
	std::queue<Point> q;
	q.push(start_pos);
	UnorderedArray<Point> farthest_positions;
	farthest_positions.push(start_pos);
	unsigned int farthes_steps_count = 0;
	while (q.size() > 0) {
		Point pos = q.front();
		q.pop();
		unsigned int new_step_index = steps[pos] + 1;
		for (Point d : DD) {
			Point new_pos = pos + d;
			if (!steps.is_on_grid(new_pos) || walls_map[new_pos] || steps[new_pos] <= new_step_index) {
				continue;
			}
			steps[new_pos] = new_step_index;
			q.push(new_pos);
			if (farthes_steps_count > new_step_index) {
				continue;
			}
			if (farthes_steps_count < new_step_index) {
				farthest_positions.clear();
			}
			farthest_positions.push(new_pos);
		}
	}
	return farthest_positions.pop();
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
		process_point(points, grid);
	}
	std::function<bool(gl::CELL)> map_function = [](gl::CELL cell) -> bool {
		return cell == gl::CELL::CLOSED || cell == gl::CELL::EMPTY;
	};
	Grid<bool> walls_map = grid.map(map_function);
	return Chamber(walls_map, start_pos, get_exit_pos(walls_map, start_pos));
}
