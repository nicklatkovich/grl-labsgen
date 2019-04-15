#pragma once

class Point
{
public:
	int x, y;
	Point();
	Point(const int x, const int y);
	Point& operator+=(const Point& other);
	Point operator+(const Point& other);
	const bool operator==(const Point& other) const;
};
