#include <genPsi/incrementalPsi.hpp>
#include "compressfa/extractfa.hpp"

#include <string>
#include <iostream>
#include <tuple>
#include <list>

#include <divsufsort.h>
#include <stdlib.h>
#include <string.h>

#include "hsds/bit-vector.hpp"

#define SAFEL 10 * 1024

/**
 * \brief iteratively generate the complete Psi array of the string
 * \details every iteration, we construct the Psi array using the already constructed Psi array and the newly added segment, the result of the current iteration will be  used as the Psi_b of the next iteration, in this step by step way we can save a lot of space and make the construction of the Psi array of huge size of input string possible
 * 
 * \param Psi_b the generated Psi array in the last iteration
 * \param seg the segment used in the last iteration
 * \param new_seg the segment used in the current iteration
 * \param B the segment have already been operated on since the initial iteration
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space 
 * \param old_alpha the map which indicates the alpha value in the computed segment before, used in get_L
 * \param old_gamma the map which indicates the gamma value in the computed segment before, used in get_L
 * \return the Psi array after adding the information of the new_seg, which will be used as Psi_b in the next iterationz
 */
std::vector<int> next_Psi(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa,
						std::map<char, int> & old_alpha, std::map<char, int> & old_gamma) {
	// M stores the rank of long suffixes among themselves
	std::vector<int> M = get_M(Psi_b, seg, new_seg, fa);
	// L stores the rank of long suffixes among S(B)
	std::vector<int> L = get_L(Psi_b, seg, new_seg, B, fa, old_alpha, old_gamma);
	// the inverse of M
	std::vector<int> invM(M.size());
	for (int i = 0; i < M.size(); ++i) {
		invM[M[i]] = i;
	}
	std::vector<int> sorted_L = L;
	std::sort(sorted_L.begin(), sorted_L.end());
	// here we construct the V vector, its len m is the length of B add the length of new_seg,
	// V is a bit vector that V[i] is 1 if the suffix of A with rank i is a long suffix and V[i]
	// is 0 otherwise, here i use hsds/bit-vector to serve as V, the library is built by Hideaki Ohno
	// std::cout << "start build V\n";
	hsds::BitVector V;
	int V_index = 0;
	// to fill in V, we first sort L into r0, r1,..., r_(l-1), then we set r0 zeros, 1 one, r1-r0 zeros, 
	// 1 one, ... and finally r_(l-1) - r_(l-2) zeros and 1 one, followed by all zeros
	for (int i = 0; i < sorted_L[0]; ++i) {
		// r0 zeros
		V.set(V_index, false);
		V_index += 1;
	}
	// 1 one
	V.set(V_index, true);
	V_index += 1;
	for (int i = 0; i < new_seg.get_length() - 1; ++i) {
		for (int j = 0; j < (sorted_L[i+1] - sorted_L[i]); ++j) {
			// r_(i+1) - r_(i) zeros
			V.set(V_index, false);
			V_index += 1;
		}
		// 1 one
		V.set(V_index, true);
		V_index += 1;
	}
	// all the left are zeros
	while (V_index < new_seg.get_length() + B.get_length()) {
		V.set(V_index, false);
		V_index += 1;
	}
	V.build(false ,true);

	std::vector<int> Psi_a(new_seg.get_length() + B.get_length());
	Psi_a[0] = M[0] + L[0];
	for (int r = 1; r < new_seg.get_length() + B.get_length(); ++r) {
		if (!V[r]) {
			uint64_t r_prime = V.rank0(r);
			int p = (*Psi_b)[r_prime];
			Psi_a[r] = V.select0(p);
		} else {
			uint64_t r_prime = V.rank1(r);
			int k = invM[r_prime];
			if (k < new_seg.get_length() - 1) {
				Psi_a[r] = M[k+1] + L[k+1];
			} else {
				int p = (*Psi_b)[0];
				Psi_a[r] = V.select0(p);
			}
		}
	}
	return Psi_a;
}

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
						std::vector<uint8_t> * const fa) {
	std::vector<int> result(new_seg.get_length());
	// P stores the rank of A_k among LS(A) when only the first l characters are considered
	std::vector<int> P = get_P(seg, new_seg, fa);
	// Q stores the rank of B_k among S(B), which equals to invSA_B[k]
	std::vector<int> Q = get_Q(Psi_b, new_seg.get_length());
	// we are interested in the rank of the tuple(P[i], Q[i])
	std::list< std::tuple<int, int, int> > sorting_list;
	for (int i = 0; i < new_seg.get_length(); ++i) {
		sorting_list.push_back(std::make_tuple(P[i], Q[i], i));
	}
	sorting_list.sort();
	int index = 0;
	for (auto x : sorting_list) {
		result[std::get<2>(x)] = index ++;
	}
	return result;
}

/**
 * \brief get the rank of A_k among LS(A) when only first l characters are considered
 * \details according to Hon's paper, here they used Larsson and Sadakane's suffix sorting algorithm to get the rank of the l segments whose length are all l, and the time complexity is O(l*logl), here  I used the open source library implemented by Yuta Mori to do the suffix sorting, which seems to be more efficient than Larsson and Safakane's algorithm
 * 
 * \param seg the segment with length l in the end
 * \param new_seg the segment with length l in the front
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space
 * \return the vector stored the rank of the A_k among LS(A)
 */
std::vector<int> get_P(segment seg, segment new_seg, std::vector<uint8_t> * const fa) {
	std::vector<int> result(new_seg.get_length());

	std::string front = "";
	std::string back = "";
	std::string two_chunk;
	// construct the string represented by new_seg
	for (int i = new_seg.get_start_index(); i < new_seg.get_end_index() + 1; ++i) {
		front += extractfa(fa, i);
	}
	// construct the string represented by seg
	for (int i = seg.get_start_index(); i < seg.get_end_index() + 1; ++i) {
		back += extractfa(fa, i);
	}
	two_chunk = front + back;
	if (two_chunk.length() > SAFEL) {
		// 2l is too long to directly sort the l strings, so use the divsufsort to save space
		// also the probability of going wrong is some  very low
		// #################### DIVSUFSORT SECTION ####################
		const char *two_chunk_char = two_chunk.c_str();
		int two_chunk_length = strlen(two_chunk_char);
		int *SA = (int *) malloc(two_chunk_length * sizeof(int));
		divsufsort((unsigned char *)two_chunk_char, SA, two_chunk_length);
		std::vector<int> invSA(two_chunk_length);
		// used to sort the rank of the first l elements in invSA
		std::list< std::tuple<int, int> > sorting_list;
		// first construct the invSA
		for (int i = 0; i < two_chunk_length; ++i) {
			invSA[SA[i]] = i;
		}
		for (int i = 0; i < new_seg.get_length(); ++i) {
			sorting_list.push_back(std::make_tuple(invSA[i], i));
		}
		sorting_list.sort();
		int index = 0;
		for (auto x : sorting_list) {
			result[std::get<1>(x)] = index ++;
		}
		free(SA);
		// ################## DIVSUFSORT SECTION END ###################
	} else {
		// 2l is short enough, so we can directly sort them, and in this case, the relative result 
		// in SA may not the real result in P
		std::list< std::tuple<std::string, int> > sorting_list;
		for (int i = 0; i < new_seg.get_length(); ++i) {
			sorting_list.push_back(std::make_tuple(two_chunk.substr(i, new_seg.get_length()), i));
		}
		sorting_list.sort();
		int index = 0;
		for (auto x : sorting_list) {
			result[std::get<1>(x)] = index ++;
		}
	}

	return result;
}

/**
 * \brief Q stores the rank of B_k among S(B), which equals to invSA_B[k]
 * \details for any k in [0,...,l-1], Q[k] = Rank(B_k, S(B)), which equals to invSA_B[k], and invSA_B[0] = Psi_b[0], invSA_B[1] = Psi_b[Psi_b[0]], etc
 * 
 * \param Psi_b the pointer to the contricted Psi array
 * \param k the length of the new_seg
 * 
 * \return the Q vector stored the rank of B_k among S(B)
 */
std::vector<int> get_Q(std::vector<int> * const Psi_b, int k) {
	std::vector<int> result(k);
	// the initial seed
	int index = (*Psi_b)[0];
	for (int i = 0; i < k; ++i) {
		result[i] = index;
		index = (*Psi_b)[index];
	}
	return result;
}

/**
 * \brief L stores the rank of long suffixes among S(B)
 * \details when Psi_b is given, the rank of the l long suffixes of A among all suffixes of B can be computed in O(l*logn + n) time, to compute L(k), we find the maximum r in range Rbc satisfying Pis_b[r] <= L[k+1], since Psi_b is strictly increaing in range Rbc, we can use binary search to find the maximum r, if r exists, we set L[k] to be r, otherwise we set L[k] to be alpha_B(c)
 * 
 * \param Psi_b the pointer to the contricted Psi array
 * \param seg the segment with length l in the end
 * \param new_seg the segment with length l in the front
 * \param B the segment have already been operated on since the initial iteration
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space
 * \param old_alpha the map which indicates the alpha value in the computed segment before, used in get_alpha_gamma
 * \param old_gamma the map which indicates the gamma value in the computed segment before, used in get_alpha_gamma
 * \return the L vector stores the rank of long suffixes among S(B)
 */
std::vector<int> get_L(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa,
						std::map<char, int> & old_alpha, std::map<char, int> & old_gamma) {
	std::vector<int> result(new_seg.get_length());

		// number of suffixes of B starting with char c
		int alpha = 0;
		// number of suffixes of B whose starting char is smaller than char c
		int gamma = 0;
		get_alpha_gamma(seg, fa, old_alpha, old_gamma);

	int k = new_seg.get_length() - 1;
	// we want to find the max r in range RBc that Psi_b(r) <= benchmark
	int benchmark;
	// we use this vector to store the ranks r that satisfy Psi_b(r) <= benchmark
	std::vector<int> rank_vector(0);
	while (k >= 0) {
		// get one char of new seg from end to head
		char c = extractfa(fa, new_seg.get_start_index() + k);
		// traverse B once and store the result in alpha and gamma
		alpha = old_alpha[c];
		gamma = old_gamma[c];
		range RBc(alpha, alpha + gamma);
		// RBc.print();
		if (k == new_seg.get_length() - 1) {
			// the initial iteration
			benchmark = (*Psi_b)[0];
		} else {
			// the element computed in last iteration
			benchmark = result[k + 1];
		}
		if (gamma != 0) {
			rank_vector = bin_search_L(RBc, Psi_b, benchmark);
		} else {
			rank_vector.clear();
		}
		if (rank_vector.size() == 0) {
			result[k] = alpha;
		} else {
			int max = -1;
			for (auto x : rank_vector) {
				if (x > max) max = x;
			}
			result[k] = max + 1;
		}
		k -= 1;
	}
	return result;
}

/**
 * \brief traverse through the seg and for each character, compute alpha and gamma value in the seg, and update the total alpha and gamma
 * \details called in get_L, look into the seg segment, get number of suffixes of seg starting with char c and number of suffixes of seg whose starting char is smaller than char c, than update the global old_alpha and old_gamma, which will be useful in get_L
 * 
 * \param seg the segment add in the last iteration, we already recorded all the information before it
 * \param fa the pointer to the fa vector which contains the information of the input string, use pointer to save space
 * \param old_alpha the map recorded the global alpha information
 * \param old_gamma the map recorded the global gamma information
 */
void get_alpha_gamma(segment seg, std::vector<uint8_t> * const fa,
					std::map<char, int> & old_alpha, std::map<char, int> & old_gamma) {
	int result = 0;
	for (int i = seg.get_start_index(); i < seg.get_end_index() + 1; ++i) {
		char x = extractfa(fa, i);
		old_gamma[x] += 1;
		if (x < 'A') old_alpha['A'] += 1;
		if (x < 'C') old_alpha['C'] += 1;
		if (x < 'G') old_alpha['G'] += 1;
		if (x < 'T') old_alpha['T'] += 1;
		if (x < 'N') old_alpha['N'] += 1;
	}
}

/**
 * \brief use binary search in Psi_b to find the max r satisfy the condition
 * \details Psi_b is strictly increasing in range RBc, so we can use binary search so efficiently find the max r
 * 
 * \param RBc the search range
 * \param Psi_b the pointer to the contricted Psi array
 * \param benchmark we want max r that Psi_b[r] <= benchmark
 * \return the rank of all the visited element that meet the condition
 */
std::vector<int> bin_search_L(range RBc, std::vector<int> * const Psi_b, int benchmark) {
	int start_index = std::get<0>(RBc.get_range());
	int end_index = std::get<1>(RBc.get_range());
	if ((*Psi_b)[end_index] < benchmark) {
		// the max in range already meet the condition
		std::vector<int> result;
		result.push_back(end_index);
		return result;
	}
	if (end_index - start_index <= 5) {
		// when the search range is small enough, we directly search through it
		std::vector<int> result;
		for (int i = start_index; i <= end_index; ++i) {
			if ((*Psi_b)[i] < benchmark) result.push_back(i);
		}
		return result;
	} else {
		// otherwise, do the binary search recursively
		int middle_index = start_index + (end_index - start_index) / 2;
		int middle_num = (*Psi_b)[middle_index];
		if (middle_num < benchmark) {
			// keep search in the second half
			range second_half(middle_index, end_index + 1);
			return bin_search_L(second_half, Psi_b, benchmark);
		} else {
			// keep search in the first half
			range first_half(start_index, middle_index + 1);
			return bin_search_L(first_half, Psi_b, benchmark);
		}
	}
}