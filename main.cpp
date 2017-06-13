#pragma warning(disable:4996)
#include <iostream>
#include "Pathfinder.h"

#define PATH 1
#define MAP_OUTPUT 0

int main()
{
	tilemap map;
	map_reader reader(map, "Map\\map.bmp");
	if (!map.initialized())
	{
		return 0;
	}

	std::vector<point> path;
	tilemap* result_map = nullptr;
#if MAP_OUTPUT
	freopen("output.txt", "w", stdout);

	tilemap _result_map;
	result_map = &_result_map;
#endif
	
	pathfinder_d dijkstra(map, path, result_map);
	dijkstra.init();
	dijkstra.find_path(map.s, map.f);

#if PATH
	for (auto& p : path)
	{
		std::cout << p.x << ", " << p.y << std::endl;
	}
#endif

#if MAP_OUTPUT
	for (int y = result_map->h-1; y >= 0; --y)
	{	
		for (int x = 0; x < result_map->w; ++x)
		{
			std::cout << (*result_map)[point(x, y)];
		}
		std::cout << std::endl;
	}
#endif

	return 0;
}
