#include <iostream>
#include <sstream>
#include <vector>

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
char extractfa(const std::vector<uint8_t> fa, const int index);