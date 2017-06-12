#include <iostream>
#include "Pathfinder.h"

void pathfinder_d::init()
{
	queue.swap(decltype(queue)());
	searched.clear();
}

bool pathfinder_d::find_path(point s, point f)
{
	find_path_impl(s, f);

	node& n = searched[f];
	while (true)
	{
		std::cout << n.pos.x << ", " << n.pos.y << std::endl;
		if (n.prev == invalid_pos)
			break;

		n = searched[n.prev];
	}

	return true;
}

bool pathfinder_d::find_path_impl(point s, point f)
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
		if (current_node.is_closed())
			continue;

		current_node.set_closed();

		static point direction[8] = { {0, -1}, {0, +1}, {-1, 0}, {+1, 0}, {-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} };
		for (int i = 0; i < 8; ++i)
		{
			double dist = (i < 4) ? d_hz : d_dg;

			dist_point next(current.pos + direction[i], current.dist + dist);
			if (!map.is_valid(next.pos.x, next.pos.y))
				continue;
			if (map[next.pos.x][next.pos.y] == etile::wall)
				continue;

			auto& next_node = searched[next.pos];
			if (next_node.is_closed())
				continue;
			if (next_node.is_opened() && next_node.dist <= next.dist)
				continue;
			next_node.prev = current.pos;
			next_node.pos = next.pos;
			next_node.dist = next.dist;
			next_node.set_opened();

			queue.emplace(next);

			if (next.pos == f)
				return true;
		}
	}

	return false;
}
