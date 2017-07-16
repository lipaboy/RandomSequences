#ifndef SEQUENCE_GENERATOR_H
#define SEQUENCE_GENERATOR_H

#include <iostream>
#include <vector>

using namespace std;

namespace PseudoRandomSequences {

	class BadArgumentException : public runtime_error {
	public:
		BadArgumentException() : runtime_error("Bad Argument Exception") {}
	};

	//typedef uint32_t uint32_t;

	//no check on wrong value (2, 3 and etc.)
	typedef uint32_t AlphabetType;
	//TODO: maybe would better storage the alphabet {0, 1} in bits
	//TODO(2): create class Sequence with incapsulate store of elements and store AlphabetSize
	typedef vector<AlphabetType> Sequence;
	//input: sequence {0, 1} ^ n with P(1) = 0.1 (possibility of 1)
	//output: sequence {0, 1} ^ n  with P(1) = 0.5

	//TODO: make template <type, int dimensionSize>, might be without first parameter
	class SequenceConverter {
	public:
		SequenceConverter(uint32_t matrixLength) { setDimension(matrixLength); }
		SequenceConverter(const string& filename) { load(filename); }

		void set(uint32_t i, uint32_t j, AlphabetType value) { rget(i, j) = value; }
		void setDimension(uint32_t dimension1);
		AlphabetType& rget(uint32_t i, uint32_t j) { return matrix[i * 2 + j]; }

		//-------Selectors--------//

		Sequence converse(const Sequence&) const;

		//TODO: think about matrix location in memory
		AlphabetType get(uint32_t i, uint32_t j) const { return matrix[i * 2 + j]; }
		uint32_t columns() const { return 2; }
		uint32_t rows() const { return rowCount; }

		void save(const string& filename, char separator = ' ');
		void load(const string& filename);

	protected:
		void setOnlyDimensionWithResizing(uint32_t dimension1);

	private:
		//matrix for conversation from sequence first type into another one
		//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows
		vector<AlphabetType> matrix;
		uint32_t rowCount;
		uint32_t dimension;
	};

	//-------Print------------//

	void printBits(ostream & o, uint32_t, uint32_t count = 32);

	ostream& operator<< (ostream & o, const SequenceConverter& seq);

	ostream& operator<< (ostream & o, const Sequence& seq);


	bool bookPileTest(const Sequence& seq);
}

#endif //SEQUENCE_GENERATOR_H
