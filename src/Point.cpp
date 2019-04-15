#include "Point.hpp"

Point::Point()
	: Point(0, 0)
{}

Point::Point(const int x, const int y)
	: x(x)
	, y(y)
{}

Point& Point::operator+=(const Point& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Point Point::operator+(const Point& other)
{
	return Point(this->x + other.x, this->y + other.y);
}

const bool Point::operator==(const Point& other) const
{
	return this->x == other.x && this->y == other.y;
}
