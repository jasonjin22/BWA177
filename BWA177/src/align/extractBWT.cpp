#include "align/extractBWT.hpp"

char extractBWT(std::vector<uint8_t> * const BWT_compressed, const int index) {
	int compressed_index = index / 2;
	int number_represent;
	if (index % 2) {
		// the least significant 4 bits
		number_represent = (*BWT_compressed)[compressed_index] & 0x0f;
	} else {
		// the most significant 4 bits
		number_represent = (*BWT_compressed)[compressed_index] & 0xf0;
		number_represent >>= 4;
	}
	switch (number_represent) {
		case 1: return 'A';
		case 2: return 'C';
		case 3: return 'G';
		case 4: return 'T';
		case 5: return 'N';
		case 0: return '$';
	}
}