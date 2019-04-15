#include "src/ChamberGener.hpp"
#include <clocale>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#define PRINT_MAP true

void write_uint(const unsigned int value, std::ostream& out)
{
	unsigned int non_const_value = value;
	char* ptr = reinterpret_cast<char*>(&non_const_value);
	for (size_t i = 0; i < sizeof(unsigned int); i++) {
		out << *ptr;
		ptr++;
	}
}

int main(int argc, char const* argv[])
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	unsigned int seed = time(0);
	ChamberGener gener(30, 16, seed);
	Chamber chamber = gener.run();
	std::ofstream file;
	std::string path_to_executable(argv[0]);
	const std::size_t last_slash_idx = path_to_executable.rfind('/');
	if (std::string::npos == last_slash_idx) {
		throw std::runtime_error("unable to get executable directory");
	}
	std::string dirname = path_to_executable.substr(0, last_slash_idx);
	file.open(dirname + std::string("/dist/maps/") + std::to_string(seed) + std::string(".grlmap"));
	write_uint(chamber.walls_map.width, file);
	write_uint(chamber.walls_map.height, file);
	unsigned int pre_height = chamber.walls_map.height - 1;
	unsigned int pre_width = chamber.walls_map.width - 1;
	char fc;
	char step = 0;
	for (unsigned int y = 0; y < chamber.walls_map.height; y++) {
		for (unsigned int x = 0; x < chamber.walls_map.width; x++) {
			step++;
			wchar_t c;
			if (chamber.is_wall(Point(x, y))) {
				c = 0x2588;
				fc = (fc << 1) + 0;
			} else {
				c = ' ';
				fc = (fc << 1) + 1;
			}
			if (step == 8 || (y == pre_height) && (x == pre_width)) {
				step = 0;
				file << fc;
			}
			if (Point(x, y) == chamber.start_pos) {
				c = 0x2592;
			}
			if (PRINT_MAP) {
				std::wcout << c << c;
			}
		}
		if (PRINT_MAP) {
			std::wcout << std::endl;
		}
	}
	write_uint(chamber.start_pos.x, file);
	write_uint(chamber.start_pos.y, file);
	file.close();
}
