#include <genPsi/incrementalPsi.hpp>
#include "compressfa/extractfa.hpp"

#include <string>
#include <iostream>
#include <tuple>
#include <list>

#include <divsufsort.h>
#include <stdlib.h>
#include <string.h>

std::vector<int> next_Psi(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa) {
	// M stores the rank of long suffixes among themselves
	std::vector<int> M = get_M(Psi_b, seg, new_seg, fa);
	// L stores the rank of long suffixes among S(B)
	std::vector<int> L = get_L(Psi_b, seg, new_seg, B, fa);
	return M;
}

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
    std::cout << "----------------\n";
    for (auto x : result) {
    	std::cout << x << std::endl;
    }
	return result;
}

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
	// std::cout << "two_chunk: " << two_chunk << std::endl;
	const char *two_chunk_char = two_chunk.c_str();

	// #################### DIVSUFSORT SECTION ####################
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
	return result;
}

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

std::vector<int> get_L(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa) {
	std::vector<int> result(new_seg.get_length());

	int k = new_seg.get_length() - 1;
	// we want to find the max r in range RBc that Psi_b(r) <= benchmark
	int benchmark;
	// we use this vector to store the ranks r that satisfy Psi_b(r) <= benchmark
	std::vector<int> rank_vector(0);
	while (k >= 0) {
		// get one char of new seg from end to head
		char c = extractfa(fa, new_seg.get_start_index() + k);
		// number of suffixes of B starting with char c
		int alpha = 0;
		// number of suffixes of B whose starting char is smaller than char c
		int gamma = 0;
		// traverse B once and store the result in alpha and gamma
		get_alpha_gamma(B, c, fa, alpha, gamma);
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
		}




		k -= 1;
	}
	return result;
}

void get_alpha_gamma(segment B, char c, std::vector<uint8_t> * const fa, int & alpha, int & gamma) {
	int result = 0;
	for (int i = B.get_start_index(); i < B.get_end_index() + 1; ++i) {
		char x = extractfa(fa, i);
		if (x == c) gamma += 1;
		if (x < c) alpha += 1;
	}
}

std::vector<int> bin_search_L(range RBc, std::vector<int> * const Psi_b, int benchmark) {}