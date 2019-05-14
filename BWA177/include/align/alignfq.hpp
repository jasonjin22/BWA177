#ifndef ALIGNFQ_HPP
#define ALIGNFQ_HPP

#include <string>
#include <vector>
#include <map>

void alignfq(std::string name, std::vector<uint8_t> * const BWT_compressed, int BWT_len,
			std::map<char, int> * const Ca, std::vector< std::map<char, int> > * Occ_chunk);

#endif