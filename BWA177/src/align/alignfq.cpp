#include "align/alignfq.hpp"
#include "align/extractBWT.hpp"
#include "align/calculateD.hpp"
#include "align/cacheOcc.hpp"
#include "utils/interval.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

/**
 * \brief read from stdin and do the align
 * \details this is the core funciton of doing the sequence alignment, it will read from the stdin and print the most likely squence
 * 
 * \param name the user given name, which should be the same as the name given in the indexing phase
 * \param BWT_compressed a pointer to the vector of the compressed BWT
 * \param BWT_len the length of the BWT string
 * \param Ca the read Ca which is in memory
 * \param Occ_chunk the read Occ which is in memory
 * \param ref_name the filename of the reference fa file, we need it to print the aligned sequence
 */
void alignfq(std::string name, std::vector<uint8_t> * const BWT_compressed, int BWT_len,
			std::map<char, int> * const Ca, std::vector< std::map<char, int> > * Occ_chunk,
			std::string ref_name) {
	std::string SA_location = "../sequence/SA/";
	SA_location += name;
	SA_location += ".SA";
	std::cout << SA_location << std::endl;
	std::ifstream SA_stream(SA_location);
	int SA;
	int SA_line = 0;
	for (std::string line; std::getline(std::cin, line); ) {
		std::vector<int> D = calculateD(line, BWT_len, Ca, Occ_chunk, BWT_compressed);
		// you can modify the z to adjust teh number of mismatch
		int z = 3;
		interval XX = inex_recur(D, line, line.length() - 1, z, 1, BWT_len - 1,
			Ca, Occ_chunk, BWT_len, BWT_compressed);
		XX.print();
		cout << endl;
		XX._list.sort();

		std::vector<int> candidate;
		bool open = false;
		int head, tail;
		for (auto x : XX._list) {
			if (std::get<1>(x) == '+') {
				head = std::get<0>(x);
			}
			if (std::get<1>(x) == '-') {
				tail = std::get<0>(x);
				for (int i = head; i <= tail; ++i) {
					candidate.push_back(i);
				}
			}
		}
		std::list<int> match_list;
		int tem = 0;
		while (SA_stream >> SA) {
			if (candidate[tem] == SA_line) {
				tem += 1;
				match_list.push_back(SA);
			}
			SA_line += 1;
			if (SA_line % 1000000 == 0) {
				cout << "SA_line: " << SA_line << endl;
			}
		}
		match_list.sort();
		for (auto x : match_list) {
			cout << x << endl;
		}
		std::string ref = "../sequence/";
		ref += ref_name;
		ref += ".fa";
		std::ifstream fin(ref);
		string str;
		int sum = 0;
		std::list<int>::iterator itr = match_list.begin();
		bool next = false;
		while ( std::getline(fin,str) ) {
			if (str[0] == '>') {
				continue;	
			}
			sum += str.length();
			if (sum > *itr) {
				cout << str << endl;
				itr ++;
				if (itr == match_list.end()) break;
			}
		} 
	}
}

/**
 * \brief recusively search the interval which satisfies the limitation
 * \details for details, pls refer to Heng Li's BWA paper, this function i just implemented the backtrack algorithm mentioned in that papre
 * 
 * \param D the calculated D array before the actual alignment
 * \param W the short read sequence we read
 * \param i funtional parameter
 * \param z funtional parameter
 * \param k funtional parameter
 * \param l funtional parameter
 * \param Ca the Ca map in memoey
 * \param Occ_chunk the Occ_chunk in memory
 * \param reference_length the BWT length
 * \param BWT_compressed the compressed_BWT
 * \return the interval contain the SA number which is matched
 */
interval inex_recur(std::vector<int> D, std::string W, int i, int z, int k, int l,
	std::map<char, int> * const Ca, std::vector< std::map<char, int> > * Occ_chunk,
	int reference_length, std::vector<uint8_t> * const BWT_compressed) {
	if (z < D[i]) {
		// return the empty interval
		// cout << "()\n";
		return interval();
	}
	if (i < 0) {
		interval x(k, l);
		// x.print();
		return x;
	}
	interval I = interval();
	I = I.interval_union(inex_recur(D, W, i - 1, z - 1, k, l, Ca, Occ_chunk, reference_length, BWT_compressed));
	// this should use deepcopy in Python
	int my_k = k;
	int my_l = l;
	std::string ACGT = "ACGT";
	for (char s : ACGT) {
		int k = (*Ca)[s] + Occ(s, my_k - 1, Occ_chunk, BWT_compressed, reference_length) + 1;
		int l = (*Ca)[s] + Occ(s, my_l, Occ_chunk, BWT_compressed, reference_length);
		if (k <= l) {
			I = I.interval_union(inex_recur(D, W, i, z - 1, k, l, Ca, Occ_chunk, reference_length, BWT_compressed));
			if (s == W[i]) {
				I = I.interval_union(inex_recur(D, W, i - 1, z, k, l, Ca, Occ_chunk, reference_length, BWT_compressed));
			} else {
				I = I.interval_union(inex_recur(D, W, i - 1, z - 1, k, l, Ca, Occ_chunk, reference_length, BWT_compressed));
			}
		}
	}
	// I.print();
	return I;
}