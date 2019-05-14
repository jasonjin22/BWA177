#ifndef ALIGNFQ_HPP
#define ALIGNFQ_HPP

#include "utils/interval.hpp"

#include <string>
#include <vector>
#include <map>

void alignfq(std::string name, std::vector<uint8_t> * const BWT_compressed, int BWT_len,
			std::map<char, int> * const Ca, std::vector< std::map<char, int> > * Occ_chunk,
			std::string ref_name);

interval inex_recur(std::vector<int> D, std::string W, int i, int z, int k, int l,
	std::map<char, int> * const Ca, std::vector< std::map<char, int> > * Occ_chunk,
	int reference_length, std::vector<uint8_t> * const BWT_compressed);

#endif