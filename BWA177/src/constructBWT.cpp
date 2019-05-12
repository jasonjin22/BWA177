#include "genBWT/constructBWT.hpp"
#include "compressfa/extractfa.hpp"

#include <string>
#include <iostream>
#include <fstream>

void constructBWT(std::vector<int> * const Psi, std::vector<uint8_t> * const fa, std::string BWTname) {
	std::string BWT((*Psi).size(), '0');
	int p = (*Psi)[0];
	int index = p; 
	// BWT[p] = 
	for (int k = 1; k <= (*Psi).size(); ++k) {
		index = (*Psi)[index];
		BWT[index] = extractfa(fa, k-1);
	}
	std::ofstream BWTFILE;
	std::string filename = "../sequence/";
	filename += BWTname;
	filename += ".BWT";
	BWTFILE.open(filename);
	BWTFILE << BWT;
	BWTFILE.close();
}