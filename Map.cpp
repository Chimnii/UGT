#include "Map.h"
#include "Bitmap.h"

char pixel_to_tile(bitmap::ecolor p)
{
	if (p == bitmap::ecolor::white)		return etile::empty;
	if (p == bitmap::ecolor::red)		return etile::start;
	if (p == bitmap::ecolor::blue)		return etile::finish;
	return etile::wall;
}

bitmap::ecolor tile_to_pixel(char t)
{
	if (t == etile::empty)	return bitmap::ecolor::white;
	if (t == etile::open)	return bitmap::ecolor::yellow;
	if (t == etile::close)	return bitmap::ecolor::green;
	if (t == etile::path)	return bitmap::ecolor::red;
	if (t == etile::start)	return bitmap::ecolor::red;
	if (t == etile::finish)	return bitmap::ecolor::blue;
	if (t == etile::wall)	return bitmap::ecolor::black;

	return bitmap::black;
}

char* tilemap::operator[](int x)
{
	return &map[x * h];
}

char& tilemap::operator[](point p)
{
	return map[p.x * h + p.y];
}

bool tilemap::is_valid(point pos)
{
	return pos.x >= 0 && pos.y >= 0 && pos.x < w && pos.y < h;
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
			bitmap::ecolor pixel = bmp.get(h - y - 1, x);

			map.map[index] = pixel_to_tile(pixel);
			if (map.map[index] == etile::start) map.s = point(x ,y);
			if (map.map[index] == etile::finish) map.f = point(x, y);
		}
	}
}



map_writer::map_writer(tilemap& map, const std::string& original_filename, const std::string& filename)
{
	bitmap bmp;
	if (!bmp.read(original_filename.c_str()))
	{
		return;
	}

	int w = bmp.width();
	int h = bmp.height();

	for (int x = 0; x < w; ++x)
	{
		for (int y = 0; y < h; ++y)
		{
			int index = x * h + y;

			bitmap::ecolor color = tile_to_pixel(map.map[index]);
			bmp.set(h - y - 1, x, color);
		}
	}

	if (!bmp.write(filename.c_str()))
	{
		return;
	}
}
