#include <iostream>
#include <cmath>
#include <map>

#include "compressfa/compressfa.hpp"
#include "compressfa/extractfa.hpp"

#include "utils/segment.hpp"
#include "genPsi/initPsi.hpp"
#include "genPsi/incrementalPsi.hpp"

#include <divsufsort.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[]) {

	std::string parameter = argv[1];

	if (parameter == "index") {
		int fa_len;
		std::cout << "start compressfa\n";
		std::vector<uint8_t> fa = compressfa(fa_len);

		// how long each segment should be when we are constructing the Psi
		int seg_length = fa_len / std::log2(fa_len);
		// the length of the first chunk to build the initial Psi
		int initial_length = fa_len % seg_length + seg_length;
		// how many iterations in all to build the whole Psi
		int iterations = fa_len / seg_length;

		// std::string initial_segment = "";
		segment seg(fa_len - initial_length, fa_len - 1);
		segment B(fa_len - initial_length, fa_len - 1);

		std::cout << "computing the initial Psi\n";
		std::vector<int> pre_Psi(seg.get_length());

		if (seg.get_length() < 20000) {
			// this case it will consume 400M to sort
			// use brute_force to build the initial small chunk of Psi array
			pre_Psi = brute_force_init_Psi(&fa, seg);
		} else {
			// in this case, we can not directly sort the segment, so i use divsufsort library
			// which provides a memory saving way to construct the suffix array
			// construct the string represented by new_seg
			std::string init_str = "";
			for (int i = seg.get_start_index(); i < seg.get_end_index() + 1; ++i) {
				init_str += extractfa(&fa, i);
			}
			// #################### DIVSUFSORT SECTION ####################
			const char *init_char = init_str.c_str();
			int init_str_len = strlen(init_char);
			int *SA = (int *) malloc(init_str_len * sizeof(int));
			divsufsort((unsigned char *)init_char, SA, init_str_len);
			std::vector<int> invSA(init_str_len);
			// used to sort the rank of the first l elements in invSA
			std::list< std::tuple<int, int> > sorting_list;
			// first construct the invSA
			for (int i = 0; i < init_str_len; ++i) {
				invSA[SA[i]] = i;
			}
			pre_Psi[0] = invSA[0];
			for (int i = 1; i < init_str_len; ++i) {
				pre_Psi[i] = invSA[SA[i]+1];
			}
			free(SA);
			// ################## DIVSUFSORT SECTION END ###################
		}

		std::vector<int> Psi_a;
		// used to denote the start and end of the segment in each iteration
		int head_index = fa_len - initial_length - seg_length;
		int tail_index = fa_len - initial_length - 1;
		// used to store the alpha and gamma value and free us from traverse through the string again and again
		std::map<char, int> old_alpha;
		std::map<char, int> old_gamma;
		old_alpha['A'] = 0;
		old_alpha['C'] = 0;
		old_alpha['G'] = 0;
		old_alpha['T'] = 0;
		old_alpha['N'] = 0;
		old_alpha['$'] = 0;

		old_gamma['A'] = 0;
		old_gamma['C'] = 0;
		old_gamma['G'] = 0;
		old_gamma['T'] = 0;
		old_gamma['N'] = 0;
		old_gamma['$'] = 0;
		// for each chunk, we construct the Psi array iteratively
		for (int i = 1; i < iterations; ++i) {
			std::cout << "iteration " << i << " / " << iterations << std::endl;
			segment new_seg(head_index, tail_index);
			head_index -= seg_length;
			tail_index -= seg_length;
			Psi_a = next_Psi(&pre_Psi, seg, new_seg, B, &fa, old_alpha, old_gamma);
			pre_Psi = Psi_a;
			seg = new_seg;
			B.increment_head(seg);
			std::cout << "length of the Psi array: " << pre_Psi.size() << std::endl;
		}
	// std::cout << "~~~~~~~~~~~~~~~pre_Psi: ~~~~~~~~~~~~~\n";
	// for (auto x : pre_Psi) std::cout << x << " ";
	// std::cout << "\n";


	} else if (parameter == "test") {
		// testssort();
	}
	return 0;
}

// ./BWA177: error while loading shared libraries: libdivsufsort.so.3: cannot open shared object file: No such file or directory

