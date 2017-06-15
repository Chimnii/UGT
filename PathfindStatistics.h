#pragma once
#include <chrono>

class pathfind_statistics
{
public:
	pathfind_statistics();
	void clear();
	void incr_opened();
	void incr_closed();
	void incr_jumped();
	void set_begin_time();
	void set_end_time();

	void set_length(double len) { length = len; }
	double get_length() { return length; }
	int get_open_node() { return open_node; }
	int get_close_node() { return close_node; }
	int get_jump_node() { return jump_node; }
	long long get_elapsed_time_ms() { return elapsed_time; }

private:
	double length;
	int open_node;
	int close_node;
	int jump_node;

	std::chrono::time_point<std::chrono::steady_clock> begin_time, end_time;
	long long elapsed_time;
};

