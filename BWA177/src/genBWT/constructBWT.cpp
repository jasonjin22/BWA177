#include "genBWT/constructBWT.hpp"
#include "compressfa/extractfa.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

/**
 * \brief generate the BWT of the input fa file and store it
 * \details use the computed Psi array and the initial string, we can build the BWT of the initial string, in O(n) time, and constructing the BWT string, we store it into the sequence directory  for further alignment, also we will store the C array and the suffix array in the disk which will be needed when doing alignment
 * 
 * \param Psi the Psi array which is built iteratively before
 * \param fa the pointer to the compressed input string, use extractfa() to read its content
 * \param BWTname user will name the BWT which helps his further alignment operation
 */
void constructBWT(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname) {
	std::string BWT((*Psi).size(), '0');
	std::vector<int> suffix_array((*Psi).size());
	std::ofstream SAFILE;
	std::string SAFILE_name = "../sequence/SA/";
	SAFILE_name += BWTname;
	SAFILE_name += ".SA";
	SAFILE.open(SAFILE_name);
	int p = (*Psi)[0];
	int index = p;
	suffix_array[0] = (*Psi).size();
	// generate the BWT file from Psi and the original string
	for (int k = 1; k <= (*Psi).size(); ++k) {
		index = (*Psi)[index];
		BWT[index] = extractfa(fa, k-1);
		suffix_array[index] = k;
	}
	// write the suffix array into the .SA file which will be used in alignment
	for (auto x : suffix_array) {
		SAFILE << x << std::endl;
	}
	SAFILE.close();
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

/**
 * \brief count and store the C(a) which will be used in alignment
 * \details iterate through the original fa file and for each A,C,G,T,N count the number of occurences and store into the disk 
 * 
 * \param Psi the Psi array which is built iteratively before
 * \param fa the pointer to the compressed input string, use extractfa() to read its content
 * \param BWTname user will name the BWT which helps his further alignment operation
 */
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

/**
 * \brief iterate through BWT and store the Occ into disk
 * \details Occ stores how many ACGTN occurs at BWT[:i+1], we can not store all the information in memory because 16 Bytes is needed for one element in BWT, so for every chunk of size std::log2(N), I store the number of all the occurence of the ACGTN before the chunk, and when we are using it, we just iterate through the chunk and add the sunm before it, maybe a cache is needed
 * 
 * \param BWT the calculated BWT
 * \param BWTname the name given by the user, which will be the name of the Occ file
 */
void constructOcc(std::string BWT, std::string BWTname) {
	int N = BWT.length();
	int chunk_size = std::log2(N);
	int chunk_num = std::ceil(N / (double)chunk_size);

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
		// std::cout << i << std::endl;
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