#ifndef PREALIGN_HPP
#define PREALIGN_HPP

#include <string>
#include <vector>
#include <map>

std::vector<uint8_t> prealign(std::string name, int & BWT_len, std::map<char, int> & Ca,
						std::vector< std::map<char, int> > & Occ_chunk);

std::vector<uint8_t> compressBWT(std::string * x, int BWT_len);

#endif