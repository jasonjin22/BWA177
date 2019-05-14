#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <list>
#include <tuple>

// the intervals are all closed
class interval {
public:
	interval();
	interval(int start, int end);
	~interval();
	void print();
	interval interval_union(interval rhs);

	bool _empty;
	std::list< std::tuple<int, char> > _list;
};

#endif
