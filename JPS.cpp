#include "JPS.h"

direction make_direction(const point& s, const point& f)
{
	direction d(0, 0);
	if (s == invalid_pos)
		return d;

	d.x = f.x > s.x ? 1 : (f.x == s.x ? 0 : -1);
	d.y = f.y > s.y ? 1 : (f.y == s.y ? 0 : -1);
	return d;
}

void pathfinder_j::init()
{
	ipathfinder::init();

	successors.swap(decltype(successors)());
	searched.clear();
}

bool pathfinder_j::find_path_impl()
{
	find_path_with_jps();

	node& n = searched[f];
	statistics->set_length(n.dist);
	while (n.prev != invalid_pos)
	{
		direction dir = make_direction(n.pos, n.prev);
		for (point p = n.pos; p != n.prev;)
		{
			path.emplace_back(p);
			write_resultmap(p, etile::path);
			p = p + dir;
		}
		n = searched[n.prev];
	}
	std::reverse(path.begin(), path.end());

	return true;
}

bool pathfinder_j::find_path_with_jps()
{
	node start;
	start.pos = s;
	start.prev = invalid_pos;
	start.dist = 0;
	searched[start.pos] = start;
	successors.emplace(start.pos, start.dist);

	while (!successors.empty())
	{
		dist_point current = successors.top();
		successors.pop();

		if (identify_successors(current.pos))
		{
			return true;
		}
	}

	return false;
}

bool pathfinder_j::identify_successors(const point& current)
{
	auto& current_node = searched[current];
	if (is_closed(current_node))
		return false;
	set_closed(current_node);

	auto neighbors = pruned_neighbors(current, make_direction(current_node.prev, current_node.pos));
	for (auto& n : neighbors)
	{
		point next = jump(current, make_direction(current, n));
		if (next == invalid_pos)
			continue;

		double next_g = current_node.dist + dist(current, next);
		double next_f = next_g + dist(next, f);

		auto& next_node = searched[next];
		if (is_closed(next_node))
			continue;
		if (is_opened(next_node) && next_node.dist <= next_g)
			continue;
		next_node.prev = current;
		next_node.pos = next;
		next_node.dist = next_g;
		set_opened(next_node);

		successors.emplace(next, next_f);

		if (next_node.pos == f)
			return true;
	}

	return false;
}

#define CAN(p) (map.is_valid(p) && map[p] != etile::wall)
#define ADD(p) neighbors.emplace_back(p)
#define ADD_ST_IF(d) do { if (CAN(p+d)) ADD(p+d); } while(0)
#define ADD_DG_IF(d1,d2) do { if (CAN(p+d1) && CAN(p+d2) && CAN(p+d1+d2)) ADD(p+d1+d2); } while(0)
#define ADD_FORCE_IF(p,d,w) do { if (!CAN(p+w) && CAN(p+d+w)) ADD(p+d+w); } while(0)
#define NONE_ADD(h1,h2,v1,v2) do { ADD_ST_IF(h1); ADD_ST_IF(h2); ADD_ST_IF(v1); ADD_ST_IF(v2); ADD_DG_IF(h1,v1); ADD_DG_IF(h1,v2); ADD_DG_IF(h2,v1); ADD_DG_IF(h2,v2); } while(0)
#define ST_ADD(d,w1,w2) do { if (CAN(p+d)) { ADD(p+d); ADD_FORCE_IF(p,d,w1); ADD_FORCE_IF(p,d,w2); } } while(0)
#define DG_ADD(d1,d2,w1,w2) do { \
if (CAN(p+d1)) { ADD(p+d1); ADD_FORCE_IF(p,d1,w1); } \
if (CAN(p+d2)) { ADD(p+d2); ADD_FORCE_IF(p,d2,w2); } \
if (CAN(p+d1) || CAN(p+d2)) { ADD(p+d1+d2); } } while(0)

std::vector<point> pathfinder_j::pruned_neighbors(const point& pos, direction dir)
{
	std::vector<point> neighbors;

	const point& p = pos;

	if (dir == none)			NONE_ADD(right,left,up,down);
	else if (dir == right)		ST_ADD(right,up,down);
	else if (dir == left)		ST_ADD(left,up,down);
	else if (dir == up)			ST_ADD(up,right,left);
	else if (dir == down)		ST_ADD(down,right,left);
	else if (dir == rightup)	DG_ADD(right,up,down,left);
	else if (dir == rightdown)	DG_ADD(right,down,up,left);
	else if (dir == leftup)		DG_ADD(left,up,down,right);
	else if (dir == leftdown)	DG_ADD(left,down,up,right);

	return neighbors;
}

bool is_diagonal(direction dir)
{
	return (dir.x != 0 && dir.y != 0);
}
point pathfinder_j::jump(point current, direction dir)
{
	if (statistics)
		statistics->incr_jumped();

	point n = current+dir;
	if (!map.is_valid(n) || map[n] == etile::wall)
		return invalid_pos;
	if (n == f)
		return n;
	if (has_force_neighbors(n, dir))
		return n;
	if (is_diagonal(dir))
	{
		direction st_d;
		st_d = dir;
		st_d.x = 0;
		if (jump(n, st_d) != invalid_pos)
			return n;
		st_d = dir;
		st_d.y = 0;
		if (jump(n, st_d) != invalid_pos)
			return n;
	}
	if (movable(n, dir))
		return jump(n, dir);
	return invalid_pos;
}

#define CHECK_FORCE(d,w) do { if (!CAN(p+w) && CAN(p+d+w)) return true; } while(0)
#define ST_CHECK_FORCE(d,w1,w2) do { CHECK_FORCE(d,w1); CHECK_FORCE(d,w2); } while(0)
#define DG_CHECK_FORCE(d1,d2,w1,w2) do { \
if (CAN(p+d1)) { CHECK_FORCE(d1,w1); } \
if (CAN(p+d2)) { CHECK_FORCE(d2,d2); } } while(0)
bool pathfinder_j::has_force_neighbors(point pos, direction dir)
{
	const point& p = pos;

	if (dir == right)			ST_CHECK_FORCE(right,up,down);
	else if (dir == left)		ST_CHECK_FORCE(left,up,down);
	else if (dir == up)			ST_CHECK_FORCE(up,right,left);
	else if (dir == down)		ST_CHECK_FORCE(down,right,left);
	else if (dir == rightup)	DG_CHECK_FORCE(right,up,down,left);
	else if (dir == rightdown)	DG_CHECK_FORCE(right,down,up,left);
	else if (dir == leftup)		DG_CHECK_FORCE(left,up,down,right);
	else if (dir == leftdown)	DG_CHECK_FORCE(left,down,up,right);

	return false;
}
