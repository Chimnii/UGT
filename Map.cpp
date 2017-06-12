#include "Map.h"
#include "Bitmap.h"

char* tilemap::operator[](int x)
{
	return &map[x * h];
}

map_reader::map_reader(tilemap& map, const std::string& filename)
{
	bitmap bmp;
	if (!bmp.read(filename.c_str()))
	{
		return;
	}

	int w = bmp.width();
	int h = bmp.height();

	map.w = w;
	map.h = h;
	map.map.resize(w * h);
	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < h; ++y)
		{
			// x-y change for map[x][y]
			int index = x * h + y;

			// bmp's (0,0) is top-left, map's (0,0) is bottom-left
			unsigned char pixel = bmp.get(h - y - 1, x);

			map.map[index] = pixel_to_tile(pixel);
			if (map.map[index] == tilemap::etile::start) map.s = std::make_pair(x, y);
			if (map.map[index] == tilemap::etile::finish) map.f = std::make_pair(x, y);
		}
	}

	map.initialized = true;
}

char map_reader::pixel_to_tile(unsigned char p)
{
	if (p == bitmap::ecolor::white) return tilemap::etile::empty;
	if (p == bitmap::ecolor::red) return tilemap::etile::start;
	if (p == bitmap::ecolor::blue) return tilemap::etile::finish;
	return tilemap::etile::wall;
}
