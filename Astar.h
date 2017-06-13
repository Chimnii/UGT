#pragma once
#include "Pathfinder.h"

class pathfinder_a : public ipathfinder
{
public:
	pathfinder_a(tilemap& _map, std::vector<point>& _path, pathfind_statistics* _statistics, tilemap* _result_map) : ipathfinder(_map, _path, _statistics, _result_map) {}
	virtual void init();

protected:
	virtual bool find_path_impl();
};
