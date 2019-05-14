#include "utils/interval.hpp"
#include <iostream>

interval::interval() {
	_empty = true;
	// _list.push_back(std::make_tuple(0, '+'));
	// _list.push_back(std::make_tuple(0, '-'));	
}

interval::interval(int start, int end) {
	_empty = false;
	_list.push_back(std::make_tuple(start, '+'));
	_list.push_back(std::make_tuple(end, '-'));
}

interval::~interval() {}

void interval::print() {
	if (_empty) {
		std::cout << "[]";
		return;
	}
	std::cout << "{";
	for (auto x : _list) {
		if (std::get<1>(x) == '+') {
			std::cout << "[" << std::get<0>(x) << ", ";
		}
		if (std::get<1>(x) == '-') {
			std::cout << std::get<0>(x) << "]";
		}
	}
	std::cout << "}";
}

interval interval::interval_union(interval rhs) {
	std::list< std::tuple<int, char> > copy_list = _list;
		for (auto x : rhs._list) {
		copy_list.push_back(x);
	}
	copy_list.sort();

	std::list< std::tuple<int, char> > new_list;
	std::tuple<int, char> start;
	std::tuple<int, char> end;
	bool first = true;
	int plus_count = 0;
	for (auto x : copy_list) {
		if (first && std::get<1>(x) == '+') {
			first = false;
			start = x;
			plus_count += 1;
		} else {
			if (std::get<1>(x) == '+') {
				plus_count += 1;
			} else {
				plus_count -= 1;
				if (plus_count == 0) {
					first = true;
					end = x;
					new_list.push_back(start);
					new_list.push_back(end);
				}
			}
		}
	}
	interval result;
	result._list = new_list;
	if (result._list.size() > 0) result._empty = false;
	return result;
}