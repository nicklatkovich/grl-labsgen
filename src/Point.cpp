#include "Point.hpp"

Point::Point()
	: Point(0, 0)
{}

Point::Point(const int x, const int y)
	: x(x)
	, y(y)
{}

Point::Point(const Point& other)
	: Point(other.x, other.y)
{}

Point& Point::operator+=(const Point& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Point Point::operator+(const Point& other) const
{
	return Point(this->x + other.x, this->y + other.y);
}

const bool Point::operator==(const Point& other) const
{
	return this->x == other.x && this->y == other.y;
}

const bool Point::operator!=(const Point& other) const
{
	return !(*this == other);
}

const bool Point::operator<(const Point& other) const
{
	return std::hash<Point>()(*this) < std::hash<Point>()(other);
}

size_t std::hash<Point>::operator()(const Point& obj) const
{
	return hash<unsigned int>()(obj.x) ^ (hash<unsigned int>()(obj.y) * 0x9e3779b9);
}
