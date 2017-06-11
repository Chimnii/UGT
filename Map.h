#pragma once
#include <vector>
#include "Bitmap.h"

class tilemap
{
public:
	enum etile : char
	{
		empty = '.',
		wall = '#',
		start = 's',
		finish = 'f'
	};

	bool readmap(const char* filename);
	const int w() const { return width; }
	const int h() const { return height; }
	char* operator[](int x);

private:
	char pixel_to_tile(unsigned char p);
	int width, height;
	std::vector<char> map;
};
