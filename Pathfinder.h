#pragma once
#include <queue>
#include <unordered_map>
#include <functional>
#include "Map.h"

constexpr double d_hz = 1.0;
constexpr double d_dg = 1.414;

const point invalid_pos(-1, -1);

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
	ipathfinder(tilemap& _map, std::vector<point>& _path, tilemap* _result_map = nullptr) : map(_map), path(_path), result_map(_result_map) {}
	virtual void init();
	virtual bool find_path(point s, point f) = 0;

protected:
	tilemap& map;
	tilemap* result_map;
	std::vector<point>& path;
};

class pathfinder_d : public ipathfinder
{
public:
	pathfinder_d(tilemap& _map, std::vector<point>& _path, tilemap* _result_map = nullptr) : ipathfinder(_map, _path, _result_map) {}
	virtual void init();
	virtual bool find_path(point s, point f);

private:
	bool find_path_impl();
	bool is_opened(node& n);
	bool is_closed(node& n);
	void set_opened(node& n);
	void set_closed(node& n);
	void set_resultmap(point& pos, char c);
	std::priority_queue<dist_point, std::vector<dist_point>, std::greater<dist_point>> queue;
	std::unordered_map<point, node> searched;
	point s, f;
};
