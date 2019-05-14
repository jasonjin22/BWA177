#include "align/cacheOcc.hpp"
#include "align/extractBWT.hpp"

#include <cmath>
#include <iostream>

using namespace std;

int Occ(char a, int index, std::vector< std::map<char, int> > * Occ_chunk,
	std::vector<uint8_t> * const BWT_compressed, int BWT_len) {
	if (index == 0) return 0;
	int result = 0;
	int chunk_size = std::log2(BWT_len);
	int chunk_num = std::ceil(BWT_len / (double)chunk_size);
	int chunk_index = index / chunk_size;
	// cout << "chunk_size: " << chunk_size << endl;
	// cout << "BWT_len: " << BWT_len << endl;
	// cout << "index: " << index << endl;
	// cout << "chunk_index: " << chunk_index << endl;
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
		cout << "formal: " << (*Occ_chunk)[chunk_index - 1][a] << endl; 
		for (int i = chunk_size * chunk_index; i <= index; ++i) {
			if (extractBWT(BWT_compressed, i) == a) {
				result += 1;
			}
		}
	}
	return result;
}