#pragma once
#include <memory>
#include <vector>
#include <string>

namespace etile
{
	enum
	{
		empty = '.',
		wall = '#',
		start = 's',
		finish = 'f',
		open = 'o',
		close = 'c',
		path = 'p'
	};
};

struct point
{
	point() : x(-1), y(-1) {}
	point(int _x, int _y) : x(_x), y(_y) {}

	bool operator==(const point& o) const { return x == o.x && y == o.y; }
	point operator+(const point& o) const { return point(x+o.x, y+o.y); }

	int x, y;
};

struct tilemap
{
	friend class map_reader;

	char* operator[](int x);
	bool is_valid(int x, int y);
	bool initialized() { return map.size() > 0; }

	int w, h;
	point s, f;

private:
	std::vector<char> map;
};

class map_reader
{
public:
	map_reader(tilemap& map, const std::string& filename);

private:
	char pixel_to_tile(unsigned char p);
};
