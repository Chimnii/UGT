#pragma once
#include <cmath>
#include "Map.h"
#include "PathfindStatistics.h"

const double d_hz = 1.0;
const double d_dg = std::sqrt<int>(2);

const point invalid_pos(-1, -1);

double dist(const point& s, const point& f);

namespace std
{
    template<>
    struct hash<point>
    {
        size_t operator()(point const& p) const
        {
            return (p.x << 16) + p.y;
        }
    };
}

struct dist_point
{
	dist_point() : pos(), dist(0.0) {}
	dist_point(point _pos, double _dist) : pos(_pos), dist(_dist) {}
	dist_point(int _x, int _y, double _dist) : pos(_x, _y), dist(_dist) {}

	bool operator <(const dist_point& o) const { return dist < o.dist; }
	bool operator >(const dist_point& o) const { return dist > o.dist; }

	point pos;
	double dist;
};

using direction = point;

struct node
{
	point pos, prev;
	double dist;

	bool is_opened() { return opened; }
	bool is_closed() { return closed; }
	void set_opened() { opened = true; }
	void set_closed() { opened = false; closed = true; }
	bool opened = false, closed = false;
};

class ipathfinder
{
public:
	ipathfinder(tilemap& _map, std::vector<point>& _path, pathfind_statistics* _statistics = nullptr, tilemap* _result_map = nullptr);
	virtual void init();
	bool find_path(point s, point f);

protected:
	virtual bool find_path_impl() = 0;
	virtual bool movable(point p, direction d);
	virtual bool is_opened(node& n);
	virtual bool is_closed(node& n);
	virtual void set_opened(node& n);
	virtual void set_closed(node& n);
	virtual void incr_statistics(char c);
	virtual void write_resultmap(point& pos, char c);

protected:
	tilemap& map;
	std::vector<point>& path;
	pathfind_statistics* statistics;
	tilemap* result_map;
	
	point s, f;
};
