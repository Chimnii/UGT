#include "JPS.h"

void pathfinder_j::init()
{
	ipathfinder::init();
}

bool pathfinder_j::find_path_impl()
{
	find_path_with_jps();

	return true;
}

bool pathfinder_j::find_path_with_jps()
{
	return false;
}
