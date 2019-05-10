#ifndef NUCLEOTIDE_HPP
#define NUCLEOTIDE_HPP

struct nucleotide
{
	bool bit0 : 1;
	bool bit1 : 1;
	bool bit2 : 1;
	nucleotide(bool bit0, bool bit1, bool bit2)
	: bit0(bit0), bit1(bit1), bit2(bit2) {};
};

#endif