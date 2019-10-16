#include "AdjacencyList.hpp"

const bool AdjacencyList::add_edge(const Point& from, const Point& to)
{
	std::set<Point>& destinations = _map[from];
	if (destinations.find(to) != destinations.end()) {
		return false;
	}
	destinations.insert(to);
	return true;
}

const bool AdjacencyList::remove_edge(const Point& from, const Point& to)
{
	auto destinations = _map[from];
	if (destinations.find(to) == destinations.end()) {
		return false;
	}
	destinations.erase(to);
	return true;
}

const bool AdjacencyList::has_outs(const Point& from) const
{
	if (_map.find(from) == _map.end()) {
		return false;
	}
	return _map.at(from).size() != 0;
}

std::set<Point> AdjacencyList::outs(const Point& from) const
{
	return _map.at(from);
}

std::unordered_map<Point, std::set<Point>> AdjacencyList::map() const
{
	return _map;
}
