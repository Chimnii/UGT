#include <chrono>
#include "PathfindStatistics.h"

pathfind_statistics::pathfind_statistics()
{
	clear();
}

void pathfind_statistics::clear()
{
	length = 0;
	open_node = 0;
	close_node = 0;
	begin_time = decltype(begin_time)();
	end_time = decltype(end_time)();
	elapsed_time = 0;
}

void pathfind_statistics::incr_opened()
{
	++open_node;
}

void pathfind_statistics::incr_closed()
{
	++close_node;
}

void pathfind_statistics::set_begin_time()
{
	begin_time = std::chrono::steady_clock::now();
}

void pathfind_statistics::set_end_time()
{
	end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
}
