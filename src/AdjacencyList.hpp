#pragma once
#include "Point.hpp"
#include <list>
#include <unordered_map>
#include <set>

class AdjacencyList
{
private:
	std::unordered_map<Point, std::set<Point>> _map;

public:
	const bool add_edge(const Point& from, const Point& to);
	const bool remove_edge(const Point& from, const Point& to);
	const bool has_outs(const Point& from) const;
	std::set<Point> outs(const Point& from) const;
	std::unordered_map<Point, std::set<Point>> map() const;
};
