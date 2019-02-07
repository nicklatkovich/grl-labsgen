#include "Grid.hpp"
#include "Cell.hpp"
#include <assert.h>

template <class T>
Grid<T>::Grid(const unsigned int width, const unsigned int height, const T default_value)
	: width(width)
	, height(height)
{
	this->_data = new T*[this->height];
	for (unsigned int y = 0; y < this->height; y++) {
		this->_data[y] = new T[this->width];
		for (unsigned int x = 0; x < this->width; x++) {
			this->_data[y][x] = default_value;
		}
	}
}

template <class T>
Grid<T>::~Grid()
{
	for (unsigned int y = 0; y < this->height; y++) {
		delete this->_data[y];
	}
	delete this->_data;
}

template <class T>
void Grid<T>::set_borders(const T& value)
{
	unsigned int pre_width = this->width - 1;
	unsigned int pre_height = this->height - 1;
	for (unsigned int x = 0; x < this->width; x++) {
		this->_data[0][x] = this->_data[pre_height][x] = value;
	}
	for (unsigned int y = 1; y < pre_height; y++) {
		this->_data[y][0] = this->_data[y][pre_width] = value;
	}
}

template <class T>
T& Grid<T>::operator[](const UIntPoint& pos)
{
	return (*this)(pos.x, pos.y);
}

template <class T>
T& Grid<T>::operator()(const unsigned int x, const unsigned int y)
{
	assert(x < this->width);
	assert(y < this->height);
	return this->_data[y][x];
}

template <class T>
const T& Grid<T>::operator()(const unsigned int x, const unsigned int y) const
{
	assert(x < this->width);
	assert(y < this->height);
	return this->_data[y][x];
}

template class Grid<gl::CELL>;
