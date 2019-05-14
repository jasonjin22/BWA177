#include "align/alignfq.hpp"
#include "align/extractBWT.hpp"
#include "align/calculateD.hpp"
#include "utils/interval.hpp"

#include <iostream>
#include <sstream>

void alignfq(std::string name, std::vector<uint8_t> * const BWT_compressed, int BWT_len,
			std::map<char, int> * const Ca, std::vector< std::map<char, int> > * Occ_chunk) {
	for (std::string line; std::getline(std::cin, line); ) {
		std::cout << line << std::endl;
		std::vector<int> D = calculateD(line, BWT_len, Ca, Occ_chunk, BWT_compressed);
		for (auto x : D) {
			std::cout << x << "\n";
		}
	}
	std::cout << "name: " << name << std::endl;
	std::cout << "BWT_compressed: " << extractBWT(BWT_compressed, 7) << std::endl;
	std::cout << "BWT_len: " << BWT_len << std::endl;
	std::cout << "Ca: " << (*Ca)['A'] << (*Ca)['T'] << std::endl;
	std::cout << "Occ_chunk: " << (*Occ_chunk)[1]['N'] << std::endl;
	// std::cout << "BWT_len: " << BWT_len << std::endl;
	interval one(1,4);
	one.print();
	interval two(5, 12);
	two.print();
	interval three = one.interval_union(two);
	three.print();
	interval four(13, 14);
	interval five = four.interval_union(three);
	five.print();
	interval empty = interval();
	interval xx = empty.interval_union(five);
	xx.print();

}

interval inex_recur(std::vector<int> D, std::string W, int i, int z, int k, int l) {

}