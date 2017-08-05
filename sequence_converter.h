#ifndef SEQUENCE_GENERATOR_H
#define SEQUENCE_GENERATOR_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace PseudoRandomSequences {

	class BadArgumentException : public runtime_error {
	public:
		BadArgumentException() : runtime_error("Bad Argument Exception") {}
	};

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
		//matrixLength = 2 ^ dimension
		SequenceConverter(uint32_t dimension = 1) { setDimension(dimension); }
		SequenceConverter(const string& filename) { load(filename); }
		SequenceConverter(SequenceConverter&& other) { swap(*this, other); }

		
		void setDimension(uint32_t dimension1);
		uint32_t getDimension() const { return dimension; }

		//-------Selectors--------//

		Sequence converse(const Sequence&) const;
		void converse(Sequence&, const Sequence&) const;

		uint32_t rows() const { return rowCount; }

		void save(const string& filename, char separator = ' ');
		void load(const string& filename);

		const SequenceConverter& operator=(SequenceConverter other);
		friend void swap(SequenceConverter& first, SequenceConverter& second);

	protected:
		AlphabetType get(uint32_t i, uint32_t j) const { return matrix[i * 2 + j]; }
		AlphabetType& rget(uint32_t i, uint32_t j) { return matrix[i * 2 + j]; }
		void set(uint32_t i, uint32_t j, AlphabetType value) { rget(i, j) = value; }
		void setOnlyDimensionWithResizing(uint32_t dimension1);

	private:
		//TODO: you can decrease memory size if storage in bits

		//matrix for conversation from sequence first type into another one
		//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows
		vector<AlphabetType> matrix;
		uint32_t rowCount;
		uint32_t dimension;
		//Notice: if you add new variables don't forget about swap
	};

	//-------Print------------//

	void printBits(ostream & o, uint32_t, uint32_t count = 32);

	double bookStackTest(const Sequence& seq);
}

#endif //SEQUENCE_GENERATOR_H
