#ifndef CONSTRUCTBWT_HPP
#define CONSTRUCTBWT_HPP

#include <stdint.h>
#include <vector>
#include <string>

/**
 * \brief generate the BWT of the input fa file and store it
 * \details use the computed Psi array and the initial string, we can build the BWT of the initial
 * string, in O(n) time, and constructing the BWT string, we store it into the sequence directory 
 * for further alignment, also we will store the C array which will be needed when doing alignment
 * 
 * \param Psi the Psi array which is built iteratively before
 * \param fa the pointer to the compressed input string, use extractfa() to read its content
 * \param BWTname user will name the BWT which helps his further alignment operation
 */
void constructBWT(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname);

void constructCa(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname);

void constructOcc(std::string BWT, std::string BWTname);

#endif
