#include "Dijkstra.h"

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

	while (!queue.empty())
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
