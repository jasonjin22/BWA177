#include <iostream>
#include <cmath>

#include "compressfa/compressfa.hpp"
#include "compressfa/extractfa.hpp"

#include "utils/segment.hpp"
#include "genPsi/initPsi.hpp"
#include "genPsi/incrementalPsi.hpp"

// #include "hsds/bit-vector.hpp"
// #include "hello/hello.hpp"

#include <divsufsort.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// using namespace hsds;



int main(int argc, char const *argv[]) {


	std::string parameter = argv[1];

	if (parameter == "index") {
		int fa_len;
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

		// use brute_force to build the initial small chunk of Psi array
		std::vector<int> pre_Psi = brute_force_init_Psi(&fa, seg);
		std::vector<int> Psi_a;
		int head_index = fa_len - initial_length - seg_length;
		int tail_index = fa_len - initial_length - 1;
		for (int i = 1; i < iterations; ++i) {
			std::cout << "index building iteration: " << i + 1 << " / " << iterations << std::endl;			
			segment new_seg(head_index, tail_index);
			// std::cout << "new_seg: " << new_seg.get_start_index() << " and " << new_seg.get_end_index() << "\n";

			head_index -= seg_length;
			tail_index -= seg_length;
			Psi_a = next_Psi(&pre_Psi, seg, new_seg, B, &fa);
			pre_Psi = Psi_a;
			seg = new_seg;
			B.increment_head(seg);
			// std::cout << "B: " << B.get_start_index() << " and " << B.get_end_index() << "\n";
		}

	} else if (parameter == "test") {
		// hello();
		// testssort();
	}
	return 0;
}

// ./BWA177: error while loading shared libraries: libdivsufsort.so.3: cannot open shared object file: No such file or directory



// for (auto s : initPsi) {
// 	std::cout << s << std::endl;
// }

// std::cout << fa_len << std::endl;


// int testssort() {
//     // intput data
//     char *Text = "jinjiachun$";
//     int n = strlen(Text);
//     int i, j;

//     // allocate
//     int *SA = (int *)malloc(n * sizeof(int));

//     // sort
//     divsufsort((unsigned char *)Text, SA, n);

//     // output
//     for(i = 0; i < n; ++i) {
//         printf("SA[%2d] = %2d: ", i, SA[i]);
//         for(j = SA[i]; j < n; ++j) {
//             printf("%c", Text[j]);
//         }
//         // printf("$\n");
//     }

//     // deallocate
//     free(SA);

//     return 0;
// }