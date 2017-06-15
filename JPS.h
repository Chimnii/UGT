#pragma once
#include <queue>
#include <unordered_map>
#include <functional>
#include "Pathfinder.h"

class pathfinder_j : public ipathfinder
{
public:
	pathfinder_j(tilemap& _map, std::vector<point>& _path, pathfind_statistics* _statistics, tilemap* _result_map) : ipathfinder(_map, _path, _statistics, _result_map) {}
	virtual void init();

protected:
	virtual bool find_path_impl();

private:
	const direction none = direction(0, 0);
	const direction right = direction(1, 0);
	const direction left = direction(-1, 0);
	const direction up = direction(0, 1);
	const direction down = direction(0, -1);
	const direction rightup = direction(1, 1);
	const direction rightdown = direction(1, -1);
	const direction leftup = direction(-1, 1);
	const direction leftdown = direction(-1, -1);

	bool find_path_with_jps();
	bool identify_successors(const point& pos);
	std::vector<point> pruned_neighbors(const point& pos, direction dir);
	point jump(point current, direction dir);
	bool has_force_neighbors(point pos, direction dir);

	std::priority_queue<dist_point, std::vector<dist_point>, std::greater<dist_point>> successors;
	std::unordered_map<point, node> searched;
};
