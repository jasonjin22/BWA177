#include <iostream>

#include "compressfa/compressfa.hpp"
#include "compressfa/extractfa.hpp"

#include "genPsi/initPsi.hpp"

#include "hsds/bit-vector.hpp"
#include "hello/hello.hpp"


using namespace hsds;


int main(int argc, char const *argv[]) {


	std::string parameter = argv[1];

	if (parameter == "index") {
		int fa_len;
		std::vector<uint8_t> fa = compressfa(fa_len);

		std::string reconstruted_string = "";
		for (int i = 0; i < fa_len; ++i) {
			reconstruted_string += extractfa(fa, i);
		}
		std::vector<int> v = brute_force_init_Psi(reconstruted_string);
		for (auto s : v) {
			std::cout << s << std::endl;
		}
		// std::cout << fa_len << std::endl;
	} else {
		hello();
	}
	return 0;
}
