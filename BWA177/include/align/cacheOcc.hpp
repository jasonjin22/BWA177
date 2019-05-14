#ifndef CACHEOCC_HPP
#define CACHEOCC_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <string>

int Occ(char a, int index, std::vector< std::map<char, int> > * Occ_chunk,
	std::vector<uint8_t> * const BWT_compressed, int BWT_len);

#endif