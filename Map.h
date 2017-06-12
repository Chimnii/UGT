#pragma once
#include <memory>
#include <vector>
#include <string>

using point = std::pair<int, int>;

struct tilemap
{
	enum etile : char
	{
		empty = '.',
		wall = '#',
		start = 's',
		finish = 'f'
	};

	int w, h;
	point s, f;

	bool initialized;
	std::vector<char> map;

	char* operator[](int x);
};

class map_reader
{
public:
	map_reader(tilemap& map, const std::string& filename);

private:
	char pixel_to_tile(unsigned char p);
};
