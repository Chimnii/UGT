#pragma once
#include <memory>
#include <vector>
#include <string>

namespace etile
{
	enum : char
	{
		empty = '.',
		wall = '#',
		start = 'S',
		finish = 'F',
		open = '0',
		close = '1',
		path = 'P'
	};
};

struct point
{
	point() : x(-1), y(-1) {}
	point(int _x, int _y) : x(_x), y(_y) {}

	bool operator==(const point& o) const { return x == o.x && y == o.y; }
	bool operator!=(const point& o) const { return x != o.x || y != o.y; }
	point operator+(const point& o) const { return point(x+o.x, y+o.y); }

	int x, y;
};

struct tilemap
{
	friend class map_reader;
	friend class map_writer;

	char* operator[](int x);
	char& operator[](point p);
	bool is_valid(point pos);
	bool initialized() { return !map.empty(); }

	int w, h;
	point s, f;

private:
	std::vector<char> map;
};

class map_reader
{
public:
	map_reader(tilemap& map, const std::string& filename);
};

class map_writer
{
public:
	map_writer(tilemap& map, const std::string& original_filename, const std::string& filename);
};
