#include "Pathfinder.h"

ipathfinder::ipathfinder(tilemap& _map, std::vector<point>& _path, pathfind_statistics* _statistics, tilemap* _result_map)
 : map(_map), path(_path), statistics(_statistics), result_map(_result_map)
{
}

void ipathfinder::init()
{
	path.clear();

	if (statistics)
	{
		statistics->clear();
	}

	if (result_map)
	{
		*result_map = map;
	}
}

bool ipathfinder::find_path(point s, point f)
{
	this->s = s;
	this->f = f;

	if (statistics)
		statistics->set_begin_time();

	find_path_impl();

	if (statistics)
		statistics->set_end_time();

	return true;
}
