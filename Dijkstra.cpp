#include <iostream>
#include "Pathfinder.h"

void pathfinder_d::init()
{
	queue.swap(decltype(queue)());
	searched.clear();
}

bool pathfinder_d::find_path(point s, point f)
{
	this->s = s;
	this->f = f;
	find_path_impl();

#ifdef DEBUG
	std::vector<point> path;
	node& n = searched[f];
	while (n.prev != invalid_pos)
	{
		path.emplace_back(n.pos);
		n = searched[n.prev];
	}
	std::reverse(path.begin(), path.end());

	for (auto& p : path)
	{
		std::cout << p.x << ", " << p.y << std::endl;
		if (p != s && p != f)
			result_map[p] = etile::path;
	}

	freopen("output.txt", "w", stdout);
	for (int y = result_map.h-1; y >= 0; --y)
	{	
		for (int x = 0; x < result_map.w; ++x)
		{
			std::cout << result_map[point(x, y)];
		}
		std::cout << std::endl;
	}
#endif

	return true;
}

bool pathfinder_d::find_path_impl()
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
	if (n.pos != s && n.pos != f)
	{
		result_map[n.pos] = etile::open;
	}
}

void pathfinder_d::set_closed(node& n)
{
	n.set_closed();
	if (n.pos != s && n.pos != f)
	{
		result_map[n.pos] = etile::close;
	}
}