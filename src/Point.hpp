#include <functional>
#pragma once

class Point
{
public:
	int x, y;
	Point();
	Point(const int x, const int y);
	Point(const Point& other);
	Point& operator+=(const Point& other);
	Point operator+(const Point& other) const;
	const bool operator==(const Point& other) const;
	const bool operator!=(const Point& other) const;
	const bool operator<(const Point& other) const;
};

template<>
struct std::hash<Point>
{
	size_t operator()(const Point& obj) const;
};
