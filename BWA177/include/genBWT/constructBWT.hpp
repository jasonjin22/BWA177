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

/**
 * \brief count and store the C(a) which will be used in alignment
 * \details iterate through the original fa file and for each A,C,G,T,N count the number of occurences and store into the disk 
 * 
 * \param Psi the Psi array which is built iteratively before
 * \param fa the pointer to the compressed input string, use extractfa() to read its content
 * \param BWTname user will name the BWT which helps his further alignment operation
 */
void constructCa(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname);

/**
 * \brief iterate through BWT and store the Occ into disk
 * \details Occ stores how many ACGTN occurs at BWT[:i+1], we can not store all the information in memory because 16 Bytes is needed for one element in BWT, so for every chunk of size std::log2(N), I store the number of all the occurence of the ACGTN before the chunk, and when we are using it, we just iterate through the chunk and add the sunm before it, maybe a cache is needed
 * 
 * \param BWT the calculated BWT
 * \param BWTname the name given by the user, which will be the name of the Occ file
 */
void constructOcc(std::string BWT, std::string BWTname);

#endif
