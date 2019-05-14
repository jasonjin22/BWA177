#include "genBWT/constructBWT.hpp"
#include "compressfa/extractfa.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

void constructBWT(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname) {
	std::string BWT((*Psi).size(), '0');
	int p = (*Psi)[0];
	int index = p;
	// generate the BWT file from Psi and the original string
	for (int k = 1; k <= (*Psi).size(); ++k) {
		index = (*Psi)[index];
		BWT[index] = extractfa(fa, k-1);
	}
	// store the BWT into the disk
	std::ofstream BWTFILE;
	std::string BWTFILE_name = "../sequence/BWT/";
	BWTFILE_name += BWTname;
	BWTFILE_name += ".BWT";
	BWTFILE.open(BWTFILE_name);
	BWTFILE << BWT;
	BWTFILE.close();
	constructCa(Psi, fa, BWTname);
	constructOcc(BWT, BWTname);
}

void constructCa(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname) {
	// compute the C(a) which denotes # of symbols in original string[0, n-2], which are less than a
	std::map<char, int> C;
	C['A'] = 0;
	C['C'] = 0;
	C['G'] = 0;
	C['T'] = 0;
	C['N'] = 0;
	for (int i = 0; i < (*Psi).size() - 1; ++i) {
		char a = extractfa(fa, i);
		if (a < 'A') {
			C['A'] += 1;
		}
		if (a < 'C') {
			C['C'] += 1;
		}
		if (a < 'G') {
			C['G'] += 1;
		}
		if (a < 'T') {
			C['T'] += 1;
		}
		if (a < 'N') {
			C['N'] += 1;
		}
	}
	std::ofstream CFILE;
	std::string CFILE_name = "../sequence/Ca/";
	CFILE_name += BWTname;
	CFILE_name += ".Ca";
	CFILE.open(CFILE_name);
	CFILE << C['A'] << '\n';
	CFILE << C['C'] << '\n';
	CFILE << C['G'] << '\n';
	CFILE << C['T'] << '\n';
	CFILE << C['N'] << '\n';
	CFILE.close();
}

void constructOcc(std::string BWT, std::string BWTname) {
	std::cout << BWT << std::endl;
	int N = BWT.length();
	int chunk_size = std::log2(N);
	int chunk_num = std::ceil(N / (double)chunk_size);

	std::cout << "chunk_size: " << chunk_size << std::endl;
	std::cout << "chunk_num: " << chunk_num << std::endl;

	std::map<char, int> Occ_map;
	Occ_map['A'] = 0;
	Occ_map['C'] = 0;
	Occ_map['G'] = 0;
	Occ_map['T'] = 0;
	Occ_map['N'] = 0;
	Occ_map['$'] = 0;
	std::ofstream OCCFILE;
	std::string OCCFILE_name = "../sequence/Occ/";
	OCCFILE_name += BWTname;
	OCCFILE_name += ".Occ";
	OCCFILE.open(OCCFILE_name);
	// construct first chunk_num - 1 chunks
	int index = 0;
	for (int i = 0; i < chunk_num - 1; ++i) {
		for (int j = 0; j < chunk_size; ++j) {
			std::cout << i * chunk_size + j << std::endl;
			switch (BWT[i * chunk_size + j]) {
				case 'A':
				Occ_map['A'] += 1;
				continue;
				case 'C':
				Occ_map['C'] += 1;
				continue;
				case 'G':
				Occ_map['G'] += 1;
				continue;
				case 'T':
				Occ_map['T'] += 1;
				continue;
				case 'N':
				Occ_map['N'] += 1;
				continue;
				case '$':
				Occ_map['$'] += 1;
				continue;
			}
		}
		OCCFILE << Occ_map['A'] << " " << Occ_map['C'] << " " << Occ_map['G'] << " " << 
		Occ_map['T'] << " " << Occ_map['N'] << ' ' << Occ_map['$'] << "\n";
	}
	// construct the last chunk which may not has length chunk_size
	for (int i = (chunk_num - 1) * chunk_size; i < N; ++i) {
		std::cout << i << std::endl;
		switch (BWT[i]) {
			case 'A':
			Occ_map['A'] += 1;
			break;
			case 'C':
			Occ_map['C'] += 1;
			break;
			case 'G':
			Occ_map['G'] += 1;
			break;
			case 'T':
			Occ_map['T'] += 1;
			break;
			case 'N':
			Occ_map['N'] += 1;
			break;
			case '$':
			Occ_map['$'] += 1;
			break;
		}
	}
	OCCFILE << Occ_map['A'] << " " << Occ_map['C'] << " " << Occ_map['G'] << " " << 
	Occ_map['T'] << " " << Occ_map['N'] << ' ' << Occ_map['$'] << "\n";
	OCCFILE.close();
}