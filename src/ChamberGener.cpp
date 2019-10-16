#include "ChamberGener.hpp"
#include "AdjacencyList.hpp"
#include "Cell.hpp"
#include "Grid.hpp"
#include "Point.hpp"
#include "UnorderedArray.hpp"
#include "representers.hpp"
#include <cstdlib>
#include <iostream>
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

Point move(const Grid<bool>& walls_map, const Point& from, const Point& d)
{
	Point result = from;
	Point next_pos = from + d;
	while (!walls_map[next_pos]) {
		result = next_pos;
		next_pos += d;
	}
	return result;
}

UnorderedArray<Point> get_farthest_poses(const Grid<bool>& walls_map, const Point& start_pos)
{
	Grid<unsigned int> steps(walls_map.width, walls_map.height, UINT32_MAX);
	steps[start_pos] = 0;
	std::queue<Point> q;
	q.push(start_pos);
	UnorderedArray<Point> result;
	result.push(start_pos);
	unsigned int farthes_steps_count = 0;
	while (q.size() > 0) {
		Point pos = q.front();
		q.pop();
		unsigned int new_step_index = steps[pos] + 1;
		for (Point d : DD) {
			Point new_pos = move(walls_map, pos, d);
			if (new_pos == pos) {
				continue;
			}
			if (steps[new_pos] <= new_step_index) {
				continue;
			}
			steps[new_pos] = new_step_index;
			q.push(new_pos);
			if (farthes_steps_count > new_step_index) {
				continue;
			}
			if (farthes_steps_count < new_step_index) {
				farthes_steps_count = new_step_index;
				result.clear();
			}
			result.push(new_pos);
		}
	}
	return result;
}

AdjacencyList generate_adjacency_list(const Grid<bool>& walls_map, const Point& start_pos)
{
	UnorderedArray<Point> points_to_process;
	AdjacencyList result;
	points_to_process.push(start_pos);
	while (!points_to_process.empty()) {
		Point pos = points_to_process.pop();
		auto farthest_poses = get_farthest_poses(walls_map, pos);
		while (!farthest_poses.empty()) {
			auto f_pos = farthest_poses.pop();
			result.add_edge(pos, f_pos);
			if (!result.has_outs(f_pos)) {
				points_to_process.push(f_pos);
			}
		}
	}
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
		process_point(points, grid);
	}
	std::function<bool(gl::CELL)> map_function = [](gl::CELL cell) -> bool {
		return cell == gl::CELL::CLOSED || cell == gl::CELL::EMPTY;
	};
	Grid<bool> walls_map = grid.map(map_function);
	auto adjacency_list = generate_adjacency_list(walls_map, start_pos);
	{
		using namespace grlrepresenters;
		std::wcout << coord2char(start_pos.x) << coord2char(start_pos.y) << std::endl;
		for (auto adjacency_pair : adjacency_list.map()) {
			std::wcout << coord2char(adjacency_pair.first.x) << coord2char(adjacency_pair.first.y) << " => ";
			for (auto it = adjacency_pair.second.begin(); it != adjacency_pair.second.end(); ++it) {
				if (it != adjacency_pair.second.begin()) {
					std::wcout << ',';
				}
				std::wcout << coord2char(it->x) << coord2char(it->y);
			}
			std::wcout << std::endl;
		}
	}
	return Chamber(walls_map, start_pos, get_farthest_poses(walls_map, start_pos).pop());
}
