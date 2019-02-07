#include "ChamberGener.hpp"
#include "Cell.hpp"
#include "Grid.hpp"
#include "UIntPoint.hpp"
#include <cstdlib>
#include <iostream>

ChamberGener::ChamberGener(
	unsigned int width,
	unsigned int height,
	unsigned int seed)
	: width(width)
	, height(height)
	, seed(seed)
{
}

template <class T>
UIntPoint get_random_pos(const Grid<T>& grid)
{
	UIntPoint result;
	result.x = (unsigned int)rand() % (grid.width - 2) + 1;
	result.y = (unsigned int)rand() % (grid.height - 2) + 1;
	return result;
}

void print(const Grid<gl::CELL>& grid)
{
	for (unsigned int y = 0; y < grid.height; y++) {
		for (unsigned int x = 0; x < grid.width; x++) {
			switch (grid(x, y)) {
			case gl::CELL::EMPTY:
				std::cout << ' ';
				break;
			case gl::CELL::POINT:
				std::cout << '.';
				break;
			case gl::CELL::CLOSED:
				std::cout << '#';
				break;
			default:
				throw "unknwon cell";
			}
		}
		std::cout << std::endl;
	}
}

void ChamberGener::run() const
{
	srand(this->seed);
	Grid<gl::CELL> grid(this->width, this->height, gl::CELL::EMPTY);
	grid.set_borders(gl::CELL::CLOSED);
	UIntPoint start_pos = get_random_pos(grid);
	grid[start_pos] = gl::CELL::POINT;
	print(grid);
}
