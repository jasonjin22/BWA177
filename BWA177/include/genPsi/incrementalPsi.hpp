#ifndef INCREMENTALPSI_HPP
#define INCREMENTALPSI_HPP

#include "utils/segment.hpp"
#include "utils/range.hpp"

#include <stdint.h>
#include <vector>

/**
 * \brief iteratively generate the complete Psi array of the string
 * \details every iteration, we construct the Psi array using the already constructed Psi array and 
 * the newly added segment, , the result of the current iteration will be  used as the Psi_b of the 
 * next iteration, in this step by step way we can save a lot of space and make the construction of 
 * the Psi array of huge size of input string possible
 * 
 * \param Psi_b the generated Psi array in the last iteration
 * \param seg the segment used in the last iteration
 * \param new_seg the segment used in the current iteration
 * \param B the segment have already been operated on since the initial iteration
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space 
 * \return the new Psi array contains the information of the new added segment
 */
std::vector<int> next_Psi(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa);

/**
 * \brief construct the M array used in nextPsi()
 * \details M stores the rank of long suffixes among themselves, for details you can refer to the Hon's paper
 * 
 * \param Psi_b the generated Psi array in the last iteration
 * \param seg the segment used in the last iteration
 * \param new_seg the segment used in the current iteration
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space 
 * \return the M array
 */
std::vector<int> get_M(std::vector<int> * const Psi_b, segment seg, segment new_seg,
						std::vector<uint8_t> * const fa);

/**
 * \brief get the rank of A_k among LS(A) when only first l characters are considered
 * \details according to Hon's paper, here they used Larsson and Sadakane's suffix sorting algorithm
 * to get the rank of the l segments whose length are all l, and the time complexity is O(l*logl), here 
 * I used the open source library implemented by Yuta Mori to do the suffix sorting, which seems to be 
 * more efficient than Larsson and Safakane's algorithm
 * 
 * \param seg the segment with length l in the end
 * \param new_seg the segment with length l in the front
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space
 * \return the vector stored the rank of the A_k among LS(A)
 */
std::vector<int> get_P(segment seg, segment new_seg, std::vector<uint8_t> * const fa);

/**
 * \brief Q stores the rank of B_k among S(B), which equals to invSA_B[k]
 * \details for any k in [0,...,l-1], Q[k] = Rank(B_k, S(B)), which equals to invSA_B[k], 
 * and invSA_B[0] = Psi_b[0], invSA_B[1] = Psi_b[Psi_b[0]], etc
 * 
 * \param Psi_b the pointer to the contricted Psi array
 * \param k the length of the new_seg
 * 
 * \return the Q vector stored the rank of B_k among S(B)
 */
std::vector<int> get_Q(std::vector<int> * const Psi_b, int k);

/**
 * \brief L stores the rank of long suffixes among S(B)
 * \details when Psi_b is given, the rank of the l long suffixes of A among all suffixes of B can be computed in 
 * O(l*logn + n) time, to compute L(k), we find the maximum r in range Rbc satisfying Pis_b[r] <= L[k+1], since 
 * Psi_b is strictly increaing in range Rbc, we can use binary search to find the maximum r, if r exists, we set L[k] 
 * to be r, otherwise we set L[k] to be alpha_B(c)
 * 
 * \param Psi_b the pointer to the contricted Psi array
 * \param seg the segment with length l in the end
 * \param new_seg the segment with length l in the front
 * \param B the segment have already been operated on since the initial iteration
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space
 * \return the L vector stores the rank of long suffixes among S(B)
 */
std::vector<int> get_L(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa);

/**
 * \brief get number of suffixes of B starting with char c and number of suffixes of B whose starting char is smaller than char c
 * \details alpha and gamma are used to construct the range in the following steps, call the function once and compute both 
 * alpha and gamma saves time
 * 
 * \param B the segment have already been operated on since the initial iteration
 * \param c the starting char c
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space
 * \param alpha we will store result of alpha there
 * \param gamma we will store result of gamma there
 */
void get_alpha_gamma(segment B, char c, std::vector<uint8_t> * const fa, int & alpha, int & gamma);

/**
 * \brief use binary search in Psi_b to find the max r satisfy the condition
 * \details Psi_b is strictly increasing in range RBc, so we can use binary search so efficiently find the max r
 * 
 * \param RBc the search range
 * \param Psi_b the pointer to the contricted Psi array
 * \param benchmark we want max r that Psi_b[r] <= benchmark
 * \return the rank of all the visited element that meet the condition
 */
std::vector<int> bin_search_L(range RBc, std::vector<int> * const Psi_b, int benchmark);

#endif
