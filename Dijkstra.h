#pragma once
#include <queue>
#include <unordered_map>
#include <functional>
#include "Pathfinder.h"

class pathfinder_d : public ipathfinder
{
public:
	pathfinder_d(tilemap& _map, std::vector<point>& _path, pathfind_statistics* _statistics, tilemap* _result_map) : ipathfinder(_map, _path, _statistics, _result_map) {}
	virtual void init();

protected:
	virtual bool find_path_impl();

private:
	bool find_path_with_dijkstra();
	
	std::priority_queue<dist_point, std::vector<dist_point>, std::greater<dist_point>> queue;
	std::unordered_map<point, node> searched;
};
