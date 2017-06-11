#include "Map.h"
#include "Bitmap.h"

bool tilemap::readmap(const char* filename)
{
	bitmap bmp;
	if (!bmp.read(filename))
	{
		return false;
	}

	width = bmp.width();
	height = bmp.height();
	map.resize(width * height);
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			// x-y change for map[x][y]
			int index = x * height + y;

			// bmp's (0,0) is top-left, map's (0,0) is bottom-left
			unsigned char pixel = bmp.get(height - y - 1, x);

			map[index] = pixel_to_tile(pixel);
		}
	}

	return true;
}

char* tilemap::operator[](int x)
{
	return &map[x * height];
}

char tilemap::pixel_to_tile(unsigned char p)
{
	if (p == bitmap::ecolor::white) return etile::empty;
	if (p == bitmap::ecolor::red) return etile::start;
	if (p == bitmap::ecolor::blue) return etile::finish;
	return etile::wall;
}
