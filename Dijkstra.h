#pragma once
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
	bool is_opened(node& n);
	bool is_closed(node& n);
	void set_opened(node& n);
	void set_closed(node& n);
	void incr_statistics(char c);
	void write_resultmap(point& pos, char c);
	std::priority_queue<dist_point, std::vector<dist_point>, std::greater<dist_point>> queue;
	std::unordered_map<point, node> searched;
};
