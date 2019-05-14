#include "align/calculateD.hpp"
#include "align/cacheOcc.hpp"
#include <iostream>
using namespace std;

std::vector<int> calculateD(std::string short_read, int reference_length, std::map<char, int> * const Ca,
	std::vector< std::map<char, int> > * Occ_chunk, std::vector<uint8_t> * const BWT_compressed) {
	std::vector<int> result;
	int z = 0;
	int j = 0;
	for (int i = 0; i < short_read.length(); ++i) {
		if (! is_subsequence(short_read.substr(j, i - j + 1), reference_length, Ca, Occ_chunk, BWT_compressed)) {
			z = z + 1;
			j = i + 1;
		}
		result.push_back(z);
	}
	return result;
}

bool is_subsequence(std::string seq, int reference_length, std::map<char, int> * const Ca,
	std::vector< std::map<char, int> > * Occ_chunk, std::vector<uint8_t> * const BWT_compressed) {
	if (seq.length() == 0) return true;
	std::string w = "";
	std::vector<int> r1(seq.length() + 1);
	std::vector<int> r2(seq.length() + 1);
	for (int i = 1; i < seq.length() + 1; ++i) {
		w = seq[seq.length() - 1 - i] + w;
	}
	r1[0] = 1;
	r2[0] = reference_length - 1;
	cout << "seq:" << seq << endl;
	for (int i = 1; i < seq.length() + 1; ++i) {
		char a = seq[seq.length() - i];
		r1[i] = (*Ca)[a] + Occ(a, r1[i-1] - 1, Occ_chunk, BWT_compressed, reference_length) + 1;
		r2[i] = (*Ca)[a] + Occ(a, r2[i-1], Occ_chunk, BWT_compressed, reference_length);
		cout << "Ca: " << (*Ca)[a] << " i = ";
		cout << i << " r1[i]: " << r1[i] << " ";
		cout  << " r2[i]: " << r2[i] << endl;
	}
	cout << "(" << r1[seq.length()] << ", " << r2[seq.length()] << ")" << endl;
	return r1[seq.length()] <= r2[seq.length()];
}