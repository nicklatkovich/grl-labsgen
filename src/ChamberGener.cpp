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

std::list<Point> get_longest_way(const AdjacencyList& adjacency_list, const Point& start_pos)
{
	std::list<Point> result({ start_pos });
	unsigned int possible_results_count = 1;
	std::list<Point> temp({ start_pos });
	unsigned int skip = 0;
	while (true) {
		auto outs = adjacency_list.outs(*std::prev(temp.end()));
		bool updated = false;
		for (auto it = std::next(outs.begin(), skip); it != outs.end(); ++it) {
			auto next_pos = *it;
			bool possible = true;
			for (auto it = temp.begin(); it != std::prev(temp.end()); ++it) {
				if (*it != *std::prev(temp.end())) {
					continue;
				}
				if (*std::next(it) == next_pos) {
					possible = false;
					break;
				}
			}
			if (!possible) {
				continue;
			}
			temp.push_back(next_pos);
			bool possible_as_result = true;
			for (auto it = temp.begin(); it != std::prev(temp.end()); ++it) {
				if (*it == next_pos) {
					possible_as_result = false;
					break;
				}
			}
			if (possible_as_result) {
				if (result.size() < temp.size()) {
					for (auto a : temp) {
						std::wcout << ' ' << grlrepresenters::coord2char(a.x) << grlrepresenters::coord2char(a.y);
					}
					std::wcout << std::endl;
					result = temp;
					possible_results_count = 1;
				} else if (result.size() == temp.size()) {
					possible_results_count++;
					for (auto a : temp) {
						std::wcout << ' ' << grlrepresenters::coord2char(a.x) << grlrepresenters::coord2char(a.y);
					}
					std::wcout << std::endl;
					if (rand() % possible_results_count == 0) {
						result = temp;
					}
				}
			}
			updated = true;
			break;
		}
		if (updated) {
			skip = 0;
			continue;
		}
		if (temp.size() == 1) {
			break;
		}
		auto prev_last_pos = *std::prev(temp.end());
		temp.pop_back();
		auto prev_outs = adjacency_list.outs(*std::prev(temp.end()));
		skip = std::distance(prev_outs.begin(), std::next(prev_outs.find(prev_last_pos)));
	}
	return result;
}

void print_full_solution(const Grid<bool>& walls_map, const std::list<Point>& longest_way)
{
	Grid<bool> total_used(walls_map.width, walls_map.height, false);
	for (auto it = longest_way.begin(), next_it = std::next(it); next_it != longest_way.end(); it = next_it++) {
		Grid<unsigned int> steps(walls_map.width, walls_map.height, UINT32_MAX);
		steps[*it] = 0;
		UnorderedArray<Point> q1;
		UnorderedArray<Point> q2;
		q2.push(*it);
		while (true) {
			if (q1.get_length() == 0) {
				if (q2.get_length() == 0) {
					break;
				}
				q1 = q2;
				q2.clear();
			}
			Point pos = q1.pop();
			unsigned int next_step_index = steps[pos] + 1;
			for (auto d : DD) {
				Point to = move(walls_map, pos, d);
				if (steps[to] > next_step_index) {
					steps[to] = next_step_index;
					q2.push(to);
				}
			}
		}
		Point pos_it = *next_it;
		Grid<bool> used(walls_map.width, walls_map.height, false);
		used[pos_it] = true;
		while (pos_it != *it) {
			Point rand_back_way_point;
			unsigned int back_way_points_count = 0;
			Point back_dir;
			for (auto d : DD) {
				for (auto back_it = pos_it + d; !walls_map[back_it]; back_it += d) {
					if (steps[back_it] == steps[pos_it] - 1) {
						if (back_way_points_count == 0 || rand() % (back_way_points_count + 1) == 0) {
							rand_back_way_point = back_it;
							back_dir = d;
						}
						back_way_points_count++;
					}
				}
			}
			while (pos_it != rand_back_way_point) {
				pos_it += back_dir;
				used[pos_it] = true;
			}
		}
		{
			auto& out = std::wcout;
			using namespace grlrepresenters;
			out << "  ";
			for (unsigned int x = 0; x < walls_map.width; x++) {
				out << ' ' << coord2char(x);
			}
			out << std::endl;
			for (unsigned int y = 0; y < walls_map.height; y++) {
				out << ' ' << coord2char(y) << ' ';
				for (unsigned int x = 0; x < walls_map.width; x++) {
					Point pos(x, y);
					if (walls_map[pos]) {
						wchar_t c = 0x2588;
						out << c << c;
					} else if (*it == pos || *next_it == pos) {
						out << grlrepresenters::coord2char(pos.x) << grlrepresenters::coord2char(pos.y);
					} else {
						wchar_t c = used[pos] ? ' ' : (total_used[pos] ? 0x2591 : 0x2592);
						out << c << c;
					}
				}
				out << std::endl;
			}
		}
		for (unsigned int x = 0; x < walls_map.width; x++) {
			for (unsigned int y = 0; y < walls_map.height; y++) {
				Point pos(x, y);
				if (used[pos]) {
					total_used[pos] = true;
				}
			}
		}
	}
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
	auto longest_way = get_longest_way(adjacency_list, start_pos);
	print_full_solution(walls_map, longest_way);
	return Chamber(walls_map, start_pos, longest_way);
}
