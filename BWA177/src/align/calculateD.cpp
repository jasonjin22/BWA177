#include "align/calculateD.hpp"
#include "align/cacheOcc.hpp"
#include <iostream>

/**
 * \brief calculate D array for recursively alignment
 * \details used in recuinex funcion
 * 
 * \param short_read the short read which we want to do align
 * \param reference_length the lenght of BWT
 * \param Ca the Ca in memory
 * \param r the Occ_chunk in memory
 * \param BWT_compressed the compressed BWT
 * \return the calculated D array
 */
std::vector<int> calculateD(std::string short_read, int reference_length, std::map<char, int> * const Ca,
	std::vector< std::map<char, int> > * Occ_chunk, std::vector<uint8_t> * const BWT_compressed) {
	std::vector<int> result;
	int z = 0;
	int j = 0;
	for (int i = 0; i < short_read.length(); ++i) {
		if (! is_subsequence(short_read.substr(j, i - j + 1), reference_length, Ca, Occ_chunk, BWT_compressed)) {
			z = z + 1;
			j = i + 1;
		}
		result.push_back(z);
	}
	return result;
}

/**
 * \brief judge whether one string is a subsequence
 * \details used in calculate D only
 * 
 * \param seq the short sequence
 * \param reference_length the BWT length
 * \param Ca the Ca in memory
 * \param r the Occ_chink in memory
 * \param BWT_compressed the compressed BWT
 * \return whether the short read is a subsquence
 */
bool is_subsequence(std::string seq, int reference_length, std::map<char, int> * const Ca,
	std::vector< std::map<char, int> > * Occ_chunk, std::vector<uint8_t> * const BWT_compressed) {
	if (seq.length() == 0) return true;
	std::string w = "";
	std::vector<int> r1(seq.length() + 1);
	std::vector<int> r2(seq.length() + 1);
	for (int i = 1; i < seq.length() + 1; ++i) {
		w = seq[seq.length() - 1 - i] + w;
	}
	r1[0] = 1;
	r2[0] = reference_length - 1;
	for (int i = 1; i < seq.length() + 1; ++i) {
		char a = seq[seq.length() - i];
		r1[i] = (*Ca)[a] + Occ(a, r1[i-1] - 1, Occ_chunk, BWT_compressed, reference_length) + 1;
		r2[i] = (*Ca)[a] + Occ(a, r2[i-1], Occ_chunk, BWT_compressed, reference_length);
	}
	return r1[seq.length()] <= r2[seq.length()];
}