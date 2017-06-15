#pragma warning(disable:4996)
#include <iostream>
#include "macro.h"
#include "PathfindStatistics.h"
#include "Dijkstra.h"
#include "Astar.h"
#include "JPS.h"

std::vector<point> path;
pathfind_statistics statistics;
tilemap result_map;

void print();
int main()
{
#if FILE_OUTPUT
	freopen("output.txt", "w", stdout);
#endif

	tilemap map;
	map_reader reader(map, "Map\\map.bmp");
	if (!map.initialized())
	{
		return 0;
	}

#if DIJKSTRA
	pathfinder_d dijkstra(map, path, &statistics, &result_map);
	dijkstra.init();
	dijkstra.find_path(map.s, map.f);

#if STATISTICS | PATH | RESULTMAP
	std::cout << "Dijkstra:" << std::endl;
	print();
#endif	// print
#endif	// DIJKSTRA

#if ASTAR
	pathfinder_a astar(map, path, &statistics, &result_map);
	astar.init();
	astar.find_path(map.s, map.f);

#if STATISTICS | PATH | RESULTMAP
	std::cout << "A*:" << std::endl;
	print();
#endif	// print
#endif	// ASTAR

#if JPS
	pathfinder_j jps(map, path, &statistics, &result_map);
	jps.init();
	jps.find_path(map.s, map.f);

#if STATISTICS | PATH | RESULTMAP
	std::cout << "JPS:" << std::endl;
	print();
#endif	// print
#endif	// JPS

	return 0;
}

void print()
{
	#if STATISTICS
	std::cout << "path length : " << statistics.get_length() << std::endl;
	std::cout << "open node : " << statistics.get_open_node() << std::endl;
	std::cout << "close node : " << statistics.get_close_node() << std::endl;
	std::cout << "elapsed time : " << statistics.get_elapsed_time_ms() << "ms" << std::endl;
#endif

#if PATH
	for (auto& p : path)
	{
		std::cout << p.x << ", " << p.y << std::endl;
	}
#endif

#if RESULTMAP
	for (int y = result_map.h-1; y >= 0; --y)
	{	
		for (int x = 0; x < result_map.w; ++x)
		{
			std::cout << result_map[point(x, y)];
		}
		std::cout << std::endl;
	}
#endif
}