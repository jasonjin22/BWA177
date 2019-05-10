#include "utils/range.hpp"

range::range(int start, int end) : _start(start), _end(end) {}
range::~range() {}
void range::print() {
	std::cout << "range(" << _start << ", " << _end << ")" << std::endl;
}