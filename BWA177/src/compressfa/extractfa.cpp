#include "compressfa/extractfa.hpp"

/**
 * \brief get the required nucleotide with index in the compressed fa vector
 * \details first get which uint8_t the required index is in, then use bit wise operation
 * to extract which nucleotide it is. Serve as a transparent layer over the compressed vector
 * and the user
 * 
 * \param fa the compressed fa file which is stored in the memory
 * \param index denote which index of nucleotide in the fa we want
 * 
 * \return the required nucleotide
 */
char extractfa(std::vector<uint8_t> * const fa, const int index) {
	int compressed_index = index / 2;
	int number_represent;
	if (index % 2) {
		// the least significant 4 bits
		number_represent = (*fa)[compressed_index] & 0x0f;
	} else {
		// the most significant 4 bits
		number_represent = (*fa)[compressed_index] & 0xf0;
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