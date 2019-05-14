#include "compressfa/compressfa.hpp"
#include <map>

/**
 * \brief read the fa file from stdin and store all the A,C,G,T,N as a four bit number, we will add '$' in the end
 * \details the original fa file use one char to store one nucletide, which is too wasteful, 
 * to save space and make the implementation easy, I use 4 bits to store one nucletide, for 
 * every two , store their information in a uint8_t and all the fa file is stored in
 * a vector, which will be maintained in the memory
 * 
 * \param fa we will store the length of nucletides in the .fa file in fa
 * \return a vector stored all the .fa file information
 */
std::vector<uint8_t> compressfa(int & fa_len) {
	// used to store the result
	std::vector<uint8_t> result;
	// record the number of valid nucletide
	int valid_nucletide = 0;
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

	for (std::string line; std::getline(std::cin, line); ) {
		if (line[0] == '>') {
			std::cout << line << std::endl;
		} else {
			// deal the real sequence
			for (char c : line) {
				if (c == 'A' or c == 'a') {
					valid_nucletide += 1;
					if (tiktok) {
						tiktok = false;
						current_number &= (dictionary['A'] << 4);
					} else {
						tiktok = true;
						current_number |= (dictionary['A']);
						result.push_back(current_number);
						current_number = 0xff;
					}
				} else if (c == 'C' or c == 'c') {
					valid_nucletide += 1;
					if (tiktok) {
						tiktok = false;
						current_number &= (dictionary['C'] << 4);
					} else {
						tiktok = true;
						current_number |= (dictionary['C']);
						result.push_back(current_number);
						current_number = 0xff;
					}
				} else if (c == 'G' or c == 'g') {
					valid_nucletide += 1;
					if (tiktok) {
						tiktok = false;
						current_number &= (dictionary['G'] << 4);
					} else {
						tiktok = true;
						current_number |= (dictionary['G']);
						result.push_back(current_number);
						current_number = 0xff;
					}
				} else if (c == 'T' or c == 't') {
					valid_nucletide += 1;
					if (tiktok) {
						tiktok = false;
						current_number &= (dictionary['T'] << 4);
					} else {
						tiktok = true;
						current_number|= (dictionary['T']);
						result.push_back(current_number);
						current_number = 0xff;
					}
				} else if (c == 'N' or c == 'n') {
					valid_nucletide += 1;
					if (tiktok) {
						tiktok = false;
						current_number &= (dictionary['N'] << 4);
					} else {
						tiktok = true;
						current_number |=(dictionary['N']);
						result.push_back(current_number);
						current_number = 0xff;
					}
				}
			}
		}
	}
		// add '$' at the end of the whole sequence
		valid_nucletide += 1;
		if (tiktok) {
			tiktok = false;
			current_number &= (dictionary['$'] << 4);
		} else {
			tiktok = true;
			current_number |=(dictionary['$']);
			result.push_back(current_number);
			current_number = 0xff;
		}
		if (!tiktok) {
			// we have odd number of nucletide in the input, push it into the vector
			result.push_back(current_number);
		}
	fa_len = valid_nucletide;
	return result;
}