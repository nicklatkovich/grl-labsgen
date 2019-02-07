#include "src/ChamberGener.hpp"
#include <clocale>
#include <iostream>
#include <time.h>

int main(int argc, char const* argv[])
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	ChamberGener gener(30, 14, time(0));
	Chamber chamber = gener.run();
	for (unsigned int y = 0; y < chamber.walls_map.height; y++) {
		for (unsigned int x = 0; x < chamber.walls_map.width; x++) {
			wchar_t c;
			if (Point(x, y) == chamber.start_pos) {
				c = 0x2592;
			} else if (chamber.is_wall(Point(x, y))) {
				c = 0x2588;
			} else {
				c = ' ';
			}
			std::wcout << c << c;
		}
		std::wcout << std::endl;
	}
}
