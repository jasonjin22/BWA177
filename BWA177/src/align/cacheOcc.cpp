#include "align/cacheOcc.hpp"
#include "align/extractBWT.hpp"

#include <cmath>
#include <iostream>

using namespace std;

/**
 * \brief read the Occ_chunk and calculate accurate the Occ number for each i
 * \details the Occ_chunk stored the Occ value in chunks, and to get the exact value, we need to visit the little chunks and add up
 * 
 * \param a the char we want query for Occ
 * \param index the index we want query for Occ
 * \param Occ_chunk the Occ_chunk
 * \param BWT_compressed the compressedBWT
 * \param BWT_len the length of BWT
 * \return the Occ value for char a and index
 */
int Occ(char a, int index, std::vector< std::map<char, int> > * Occ_chunk,
	std::vector<uint8_t> * const BWT_compressed, int BWT_len) {
	if (index == 0) return 0;
	int result = 0;
	int chunk_size = std::log2(BWT_len);
	int chunk_num = std::ceil(BWT_len / (double)chunk_size);
	int chunk_index = index / chunk_size;
	if (chunk_index == 0) {
		// in this case we fo not refer to the precalculated Occ_chunk, 
		// instead we directly visit BWT to count
		for (int i = 0; i <= index; ++i) {
			if (extractBWT(BWT_compressed, i) == a) {
				result += 1;
			}
		}
	} else {
		// we want to refer to the precalculated Occ_chunk and visit the next chunk
		// add the number occured in the chunks before this chunk
		result += (*Occ_chunk)[chunk_index - 1][a];
		for (int i = chunk_size * chunk_index; i <= index; ++i) {
			if (extractBWT(BWT_compressed, i) == a) {
				result += 1;
			}
		}
	}
	return result;
}