#include "align/prealign.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * \brief before do the align, first load the BWT and other things calculated in the index phase into memory
 * \details load the BWT, Ca, Occ_chunk
 * 
 * \param name the name given by the user to denote the index things
 * \param BWT_len the length of the BWT, we will store the length of BWT into it
 * \param Ca we will read the Ca into Ca
 * \param Occ_chunk we will read Occ_chunk into it
 * \return return the compressed BWT as a uint8_t vector
 */
std::vector<uint8_t> prealign(std::string name, int & BWT_len, std::map<char, int> & Ca,
						std::vector< std::map<char, int> > & Occ_chunk) {
	std::string BWT_location = "../sequence/BWT/";
	BWT_location += name;
	BWT_location += ".BWT";
	std::ifstream BWT_stream(BWT_location);
	std::stringstream BWT_buffer;
	BWT_buffer << BWT_stream.rdbuf();
	std::string BWT_string(BWT_buffer.str());

	BWT_len = BWT_string.length();
	std::vector<uint8_t> result = compressBWT(&BWT_string, BWT_len);
	std::string Ca_location = "../sequence/Ca/";
	Ca_location += name;
	Ca_location += ".Ca";
	std::ifstream Ca_stream(Ca_location);
	int A, C, G, T, N;
	while (Ca_stream >> A >> C >> G >> T >> N) {
		Ca['A'] = A;
		Ca['C'] = C;
		Ca['G'] = G;
		Ca['T'] = T;
		Ca['N'] = N;
	}

	std::string Occ_location = "../sequence/Occ/";
	Occ_location += name;
	Occ_location += ".Occ";
	std::ifstream Occ_stream(Occ_location);
	int Occ_A, Occ_C, Occ_G, Occ_T, Occ_N, Occ_dollar;
	while (Occ_stream >> Occ_A >> Occ_C >> Occ_G >> Occ_T >> Occ_N >> Occ_dollar) {
		std::map<char, int> map;
		map['A'] = Occ_A;
		map['C'] = Occ_C;
		map['G'] = Occ_G;
		map['T'] = Occ_T;
		map['N'] = Occ_N;
		map['$'] = Occ_dollar;
		Occ_chunk.push_back(map);
	}

	return result;
}

std::vector<uint8_t> compressBWT(std::string * BWT_string, int BWT_len) {
	std::vector<uint8_t> result;

	// used to denote whether to store in the first 4 bits or last 4 bits
	bool tiktok = true;
	// used to store 2 nucletide;
	uint8_t current_number = 0xff;
	// note we store in the most significant 4 bits first, then store in the least significant 4 bits
	// $ = 0000, A = 0001, C = 0010, G = 0011, T = 0100, N = 0101
	std::map<char, uint8_t> dictionary;
	dictionary['A'] = 1;
	dictionary['C'] = 2;
	dictionary['G'] = 3;
	dictionary['T'] = 4;
	dictionary['N'] = 5;
	dictionary['$'] = 0;

	for (int i = 0; i < BWT_len; ++i) {
		char c = (*BWT_string)[i];
		// deal the real sequence
		if (c == 'A') {
			if (tiktok) {
				tiktok = false;
				current_number &= (dictionary['A'] << 4);
			} else {
				tiktok = true;
				current_number |= (dictionary['A']);
				result.push_back(current_number);
				current_number = 0xff;
			}
		} else if (c == 'C') {
			if (tiktok) {
				tiktok = false;
				current_number &= (dictionary['C'] << 4);
			} else {
				tiktok = true;
				current_number |= (dictionary['C']);
				result.push_back(current_number);
				current_number = 0xff;
			}
		} else if (c == 'G') {
			if (tiktok) {
				tiktok = false;
				current_number &= (dictionary['G'] << 4);
			} else {
				tiktok = true;
				current_number |= (dictionary['G']);
				result.push_back(current_number);
				current_number = 0xff;
			}
		} else if (c == 'T') {
			if (tiktok) {
				tiktok = false;
				current_number &= (dictionary['T'] << 4);
			} else {
				tiktok = true;
				current_number|= (dictionary['T']);
				result.push_back(current_number);
				current_number = 0xff;
			}
		} else if (c == 'N') {
			if (tiktok) {
				tiktok = false;
				current_number &= (dictionary['N'] << 4);
			} else {
				tiktok = true;
				current_number |=(dictionary['N']);
				result.push_back(current_number);
				current_number = 0xff;
			}
		} else if (c == '$') {
			if (tiktok) {
				tiktok = false;
				current_number &= (dictionary['$'] << 4);
			} else {
				tiktok = true;
				current_number |=(dictionary['$']);
				result.push_back(current_number);
				current_number = 0xff;
			}
		}
	}
	if (!tiktok) {
		// we have odd number of nucletide in the input, push it into the vector
		result.push_back(current_number);
	}
	return result;
}



