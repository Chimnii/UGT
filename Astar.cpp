#include "Astar.h"

void pathfinder_a::init()
{
	ipathfinder::init();

	queue.swap(decltype(queue)());
	searched.clear();
}

bool pathfinder_a::find_path_impl()
{
	find_path_with_astar();

	node& n = searched[f];
	statistics->set_length(n.dist);
	while (n.prev != invalid_pos)
	{
		path.emplace_back(n.pos);
		write_resultmap(n.pos, etile::path);
		n = searched[n.prev];
	}
	std::reverse(path.begin(), path.end());

	return true;
}

bool pathfinder_a::find_path_with_astar()
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

			point next_pos = current.pos + direction[i];
			double next_g = current_node.dist + dist;
			double next_f = next_g + hvalue(next_pos);
			if (!map.is_valid(next_pos))
				continue;
			if (map[next_pos] == etile::wall)
				continue;

			auto& next_node = searched[next_pos];
			if (is_closed(next_node))
				continue;
			if (is_opened(next_node) && next_node.dist <= next_g)
				continue;
			next_node.prev = current.pos;
			next_node.pos = next_pos;
			next_node.dist = next_g;
			set_opened(next_node);
			
			queue.emplace(next_pos, next_f);

			if (next_pos == f)
				return true;
		}
	}

	return false;
}

double pathfinder_a::hvalue(const point& p)
{
	return dist(p, f);
}
