#pragma once
#include "UIntPoint.hpp"

template <class T>
class Grid {
private:
	T** _data;

public:
	unsigned int width, height;
	Grid(const unsigned int width, const unsigned int height, const T default_value);
	~Grid();
	void set_borders(const T& value);
	T& operator[](const UIntPoint& pos);
	T& operator()(const unsigned int x, const unsigned int y);
	const T& operator()(const unsigned int x, const unsigned int y) const;
};
