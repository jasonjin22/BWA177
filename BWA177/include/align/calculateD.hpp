#ifndef CALCULATED_HPP
#define CALCULATED_HPP

#include <vector>
#include <string>
#include <map>


std::vector<int> calculateD(std::string short_read, int reference_length, std::map<char, int> * const Ca,
	std::vector< std::map<char, int> > * Occ_chunk, std::vector<uint8_t> * const BWT_compressed);

bool is_subsequence(std::string seq, int reference_length, std::map<char, int> * const Ca,
	std::vector< std::map<char, int> > * Occ_chunk, std::vector<uint8_t> * const BWT_compressed);

#endif
