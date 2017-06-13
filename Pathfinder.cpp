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

bool ipathfinder::is_opened(node& n)
{
	return n.is_opened();
}

bool ipathfinder::is_closed(node& n)
{
	return n.is_closed();
}

void ipathfinder::set_opened(node& n)
{
	n.set_opened();
	incr_statistics(etile::open);
	write_resultmap(n.pos, etile::open);
}

void ipathfinder::set_closed(node& n)
{
	n.set_closed();
	incr_statistics(etile::close);
	write_resultmap(n.pos, etile::close);
}

void ipathfinder::incr_statistics(char c)
{
	if (statistics)
	{
		if (c == etile::open)
			statistics->incr_opened();
		if (c == etile::close)
			statistics->incr_closed();
	}
}

void ipathfinder::write_resultmap(point& pos, char c)
{
	if (result_map)
	{
		if (pos != s && pos != f)
			(*result_map)[pos] = c;
	}
}
