#include <iostream>
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

void pathfinder_d::init()
{
	ipathfinder::init();

	queue.swap(decltype(queue)());
	searched.clear();
}

bool pathfinder_d::find_path_impl()
{
	find_path_with_dijkstra();

	node& n = searched[f];
	while (n.prev != invalid_pos)
	{
		path.emplace_back(n.pos);
		write_resultmap(n.pos, etile::path);
		n = searched[n.prev];
	}
	std::reverse(path.begin(), path.end());

	return true;
}

bool pathfinder_d::find_path_with_dijkstra()
{
	node start;
	start.pos = s;
	start.prev = invalid_pos;
	start.dist = 0;
	searched[start.pos] = start;
	queue.emplace(start.pos, start.dist);

	while (queue.size() > 0)
	{
		dist_point current = queue.top();
		queue.pop();

		auto& current_node = searched[current.pos];
		if (is_closed(current_node))
			continue;
		set_closed(current_node);

		static point direction[8] = { {0, -1}, {0, +1}, {-1, 0}, {+1, 0}, {-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} };
		for (int i = 0; i < 8; ++i)
		{
			double dist = (i < 4) ? d_hz : d_dg;

			dist_point next(current.pos + direction[i], current.dist + dist);
			if (!map.is_valid(next.pos))
				continue;
			if (map[next.pos] == etile::wall)
				continue;

			auto& next_node = searched[next.pos];
			if (is_closed(next_node))
				continue;
			if (is_opened(next_node) && next_node.dist <= next.dist)
				continue;
			next_node.prev = current.pos;
			next_node.pos = next.pos;
			next_node.dist = next.dist;
			set_opened(next_node);
			
			queue.emplace(next);

			if (next.pos == f)
				return true;
		}
	}

	return false;
}

bool pathfinder_d::is_opened(node& n)
{
	return n.is_opened();
}

bool pathfinder_d::is_closed(node& n)
{
	return n.is_closed();
}

void pathfinder_d::set_opened(node& n)
{
	n.set_opened();
	incr_statistics(etile::open);
	write_resultmap(n.pos, etile::open);
}

void pathfinder_d::set_closed(node& n)
{
	n.set_closed();
	incr_statistics(etile::close);
	write_resultmap(n.pos, etile::close);
}

void pathfinder_d::incr_statistics(char c)
{
	if (statistics)
	{
		if (c == etile::open)
			statistics->incr_opened();
		if (c == etile::close)
			statistics->incr_closed();
	}
}

void pathfinder_d::write_resultmap(point& pos, char c)
{
	if (result_map)
	{
		if (pos != s && pos != f)
			(*result_map)[pos] = c;
	}
}
