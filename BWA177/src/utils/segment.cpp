#include "utils/segment.hpp"

segment::segment(int start_index, int end_index) : _start_index(start_index), _end_index(end_index) {}

segment::~segment() {}

int segment::get_start_index() {
	return _start_index;
}

int segment::get_end_index() {
	return _end_index;
}

int segment::get_length() {
	return _end_index - _start_index + 1;
}

/**
 * \brief designed for the building of Psi_b, only called in main
 * \details extend the start index to a new one which can involve the newly added chunk
 * 
 * \param seg the start index of seg will be the new start index of the caller
 */
void segment::increment_head(segment seg) {
	int new_head_index = seg.get_start_index();
	_start_index = new_head_index;
}