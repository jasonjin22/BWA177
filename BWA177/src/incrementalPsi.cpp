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

std::vector<int> next_Psi(std::vector<int> * const Psi_b, segment seg, segment new_seg, 
						segment B, std::vector<uint8_t> * const fa,
						std::map<char, int> & old_alpha, std::map<char, int> & old_gamma) {
	// M stores the rank of long suffixes among themselves
	// std::cout << "start M\n";
	std::vector<int> M = get_M(Psi_b, seg, new_seg, fa);
	// std::cout << "end M\n";
	// L stores the rank of long suffixes among S(B)
	// std::cout << "start L\n";
	std::vector<int> L = get_L(Psi_b, seg, new_seg, B, fa, old_alpha, old_gamma);
	// std::cout << "end L\n";
	// the inverse of M
	std::vector<int> invM(M.size());
	for (int i = 0; i < M.size(); ++i) {
		invM[M[i]] = i;
	}
	// std::cout << "M:!!!!!!!!!!!!!!!!!!!!!1\n";
	// for (auto x : M) std::cout << x << " ";
	// std::cout << "\n";
	// std::cout << "L:!!!!!!!!!!!!!!!!!!!!!1\n";
	// for (auto x : L) std::cout << x << " ";
	// std::cout << "\n";
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
	// std::cout << "end build V\n";
	// std::cout << "V:!!!!!!!!!!!!!!!!!!!!!1\n";
	// for (int i = 0; i < new_seg.get_length() + B.get_length(); ++i) {
	// 	std::cout << V[i] << " ";
	// }
	// std::cout << "\n";
	// std::cout << "start Psi_a\n";
	std::vector<int> Psi_a(new_seg.get_length() + B.get_length());
	Psi_a[0] = M[0] + L[0];
	for (int r = 1; r < new_seg.get_length() + B.get_length(); ++r) {
		if (!V[r]) {
			uint64_t r_prime = V.rank0(r);
			int p = (*Psi_b)[r_prime];
			// if (V.select0(p) == -1) std::cout << "one\n";
			Psi_a[r] = V.select0(p);
		} else {
			uint64_t r_prime = V.rank1(r);
			int k = invM[r_prime];
			if (k < new_seg.get_length() - 1) {
				// if (M[k+1] + L[k+1] == -1) std::cout << "two\n";

				Psi_a[r] = M[k+1] + L[k+1];
			} else {
				int p = (*Psi_b)[0];
				// if (V.select0(p) == -1) std::cout << "three\n";
				Psi_a[r] = V.select0(p);
			}
		}
	}
	return Psi_a;
}

std::vector<int> get_M(std::vector<int> * const Psi_b, segment seg, segment new_seg,
						std::vector<uint8_t> * const fa) {
	std::vector<int> result(new_seg.get_length());
	// P stores the rank of A_k among LS(A) when only the first l characters are considered
	// std::cout << "start P\n";
	std::vector<int> P = get_P(seg, new_seg, fa);
	// std::cout << "end P\n";
	// std::cout << "P:!!!!!!!!!!!!!!!!!!!!!1\n";
	// for (auto x : P) std::cout << x << " ";
	// std::cout << "\n";
	// Q stores the rank of B_k among S(B), which equals to invSA_B[k]
	// std::cout << "start Q\n";
	std::vector<int> Q = get_Q(Psi_b, new_seg.get_length());
	// std::cout << "end Q\n";
	// std::cout << "Q:!!!!!!!!!!!!!!!!!!!!!1\n";
	// for (auto x : Q) std::cout << x << " ";
	// std::cout << "\n";
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
	// std::cout << "----------------\n";
	// for (auto x : result) {
	// 	std::cout << x << std::endl;
	// }
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
			// std::cout << "clear\n";
			rank_vector.clear();
		}
		// std::cout << "========================\n";
		// for (auto x : rank_vector) {
		// 	std::cout << x << " ";
		// }
		// std::cout << "\n";
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
	// alpha += old_alpha[c];
	// gamma += old_gamma[c];
	// old_alpha[c] = alpha;
	// old_gamma[c] = gamma;
	// alpha += old_alpha;
	// gamma += old_gamma;
}

std::vector<int> bin_search_L(range RBc, std::vector<int> * const Psi_b, int benchmark) {
	int start_index = std::get<0>(RBc.get_range());
	int end_index = std::get<1>(RBc.get_range());
	// std::cout << "start_index: " << start_index << std::endl;
	// std::cout << "end_index: " << end_index << std::endl;
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