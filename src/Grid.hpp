#pragma once
#include "Point.hpp"
#include "functional"

template<class T>
class Grid
{
private:
	T** _data;

public:
	unsigned int width, height;
	Grid();
	Grid(const unsigned int width, const unsigned int height);
	Grid(const unsigned int width, const unsigned int height, const T default_value);
	Grid(const Grid<T>& to_copy);
	~Grid();
	void set_borders(const T& value);
	T& operator[](const Point& pos);
	const T& operator[](const Point& pos) const;
	T& operator()(const unsigned int x, const unsigned int y);
	const T& operator()(const unsigned int x, const unsigned int y) const;
	template<class K>
	Grid<K> map(std::function<K(T)> mapFunction) const;
};
