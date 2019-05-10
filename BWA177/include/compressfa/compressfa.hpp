#ifndef COMPRESSFA_HPP
#define COMPRESSFA_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
std::vector<uint8_t> compressfa(int & fa_len);

#endif
