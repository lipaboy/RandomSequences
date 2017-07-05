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

	typedef uint32_t uint32;

	//no check on wrong value (2, 3 and etc.)
	typedef unsigned int AlphabetType;
	//TODO: maybe would better storage the alphabet {0, 1} in bits
	typedef vector<AlphabetType> Sequence;
	//input: sequence {0, 1} ^ n with P(1) = 0.1 (possibility of 1)
	//output: sequence {0, 1} ^ n  with P(1) = 0.5

	//TODO: make template <type, int dimensionSize>, might be without first parameter
	class SequenceConverter {
	public:
		SequenceConverter(uint32 matrixLength);		//k - matrix dimension, matrix length
		SequenceConverter(const string& filename);

		Sequence converse(const Sequence&) const;

		void set(uint32 i, uint32 j, AlphabetType value) { matrix[i * 2 + j] = value; }

		//-------Selectors--------//

		//TODO: think about matrix location in memory
		AlphabetType get(uint32 i, uint32 j) const { return matrix[i * 2 + j]; }
		uint32 columns() const { return 2; }
		uint32 rows() const { return rowCount; }

		void save(const string& filename, char separator = ' ');
		void load(const string& filename);

	protected:
		void resize(uint32 dimension1);

	private:
		//matrix for conversation from sequence first type into another one
		vector<AlphabetType> matrix;
		uint32 rowCount;
		uint32 dimension;
	};

	//-------Print------------//

	void printBits(ostream & o, uint32, uint32 count = 32);

	ostream& operator<< (ostream & o, const SequenceConverter& seq);

	ostream& operator<< (ostream & o, const Sequence& seq);
}

#endif //SEQUENCE_GENERATOR_H
