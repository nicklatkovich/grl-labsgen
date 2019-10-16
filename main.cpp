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

const char coord2char(const unsigned int coord)
{
	if (coord < 10) {
		return '0' + coord;
	}
	if (coord < 'Z' - 'A' + 11) {
		return 'A' + coord - 10;
	}
	throw std::runtime_error("unable to present coord");
}

void print_map(const Chamber& chamber, std::wostream& out)
{
	out << "  ";
	for (unsigned int x = 0; x < chamber.width(); x++) {
		out << ' ' << coord2char(x);
	}
	out << std::endl;
	for (unsigned int y = 0; y < chamber.height(); y++) {
		out << ' ' << coord2char(y) << ' ';
		for (unsigned int x = 0; x < chamber.width(); x++) {
			wchar_t c = chamber.is_wall(Point(x, y)) ? 0x2588 : ' ';
			out << c << c;
		}
		out << std::endl;
	}
}

void save_map(const Chamber& chamber, std::ofstream& file)
{
	write_uint(chamber.walls_map.width, file);
	write_uint(chamber.walls_map.height, file);
	unsigned int pre_height = chamber.walls_map.height - 1;
	unsigned int pre_width = chamber.walls_map.width - 1;
	char fc;
	char step = 0;
	for (unsigned int y = 0; y < chamber.walls_map.height; y++) {
		for (unsigned int x = 0; x < chamber.walls_map.width; x++) {
			step++;
			fc = (fc << 1) + (chamber.is_wall(Point(x, y)) ? 0 : 1);
			if (step == 8 || (y == pre_height) && (x == pre_width)) {
				step = 0;
				file << fc;
			}
		}
	}
	write_uint(chamber.start_pos.x, file);
	write_uint(chamber.start_pos.y, file);
	write_uint(chamber.exit_pos.x, file);
	write_uint(chamber.exit_pos.y, file);
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
	save_map(chamber, file);
	if (PRINT_MAP) {
		print_map(chamber, std::wcout);
	}
	file.close();
}
