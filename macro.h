#pragma once

#define FILE_OUTPUT 0
#define STATISTICS 1
#define PATH 0
#define RESULTMAP 0

#if RESULTMAP
#undef FILE_OUTPUT
#define FILE_OUTPUT 1
#endif

#define DIJKSTRA 1
#define ASTAR 1
#define JPS 1
