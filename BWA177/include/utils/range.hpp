#ifndef RANGE_HPP
#define RANGE_HPP

#include <iostream>

/**
 * \brief a class to simulate the range in Python
 * \details just store the start and the end, make it similar with the range in Python, includes 
 * the start and does not include the end
 * \param start start index of the range, include it
 * \param end end index of the range, does not include it
 * 
 * \return nothing
 */
class range
{
public:
	range(int start, int end);
	~range();
	void print();
private:
	int _start;
	int _end;
};

#endif