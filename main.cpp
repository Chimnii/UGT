#include <iostream>
#include "Map.h"

//#define DEBUG

int main()
{
	tilemap map;
	map_reader reader(map, "map.bmp");
	if (!map.initialized)
	{
		return 0;
	}

#ifdef DEBUG
	freopen("output.txt", "w", stdout);
	for (int y = map.h-1; y >= 0; --y)
	{	
		for (int x = 0; x < map.w; ++x)
		{
			std::cout << map[x][y];
		}
		std::cout << std::endl;
	}
#endif

	return 0;
}
