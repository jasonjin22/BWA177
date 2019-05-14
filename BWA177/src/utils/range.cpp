#include "utils/range.hpp"

range::range(int start, int end) : _start(start), _end(end) {}

range::~range() {}

void range::print() {
	std::cout << "range(" << _start << ", " << _end << ")" << std::endl;
}

std::tuple<int, int> range::get_range() {
	return std::make_tuple(_start, _end - 1);
}