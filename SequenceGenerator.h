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

	typedef unsigned int AlphabetType;
	//input: sequence {0, 1} ^ n with P(1) = 0.1 (possibility of 1)
	//output: sequence {0, 1} ^ n  with P(1) = 0.5
	class SequenceGenerator {
	public:
		SequenceGenerator(uint32 k);		//k - matrix dimension

		void set(uint32 i, uint32 j, AlphabetType value) { matrix[i * columnCount + j] = value; }

		//-------Selectors--------//

		//TODO: think about matrix location in memory
		AlphabetType get(uint32 i, uint32 j) const { return matrix[i * columnCount + j]; }
		uint32 columns() const { return columnCount; }
		uint32 rows() const { return rowCount; }

		void save(const string& filename, char separator = ' ');

		//-------Print------------//
		//void print(ostream &o = cout) const {}
		//friend ostream& operator<< (ostream & o, SequenceGenerator seq);

	private:
		//matrix for conversation from sequence first type into another one
		vector<AlphabetType> matrix;
		uint32 rowCount;
		const uint32 columnCount = 2;
	};

	ostream& operator<< (ostream & o, const SequenceGenerator& seq);
}

#endif //SEQUENCE_GENERATOR_H
