#ifndef SEQUENCE_GENERATOR_H
#define SEQUENCE_GENERATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>

#include <boost\range\any_range.hpp>

//template <typename T>
typedef boost::any_range<
	int,
	boost::bidirectional_traversal_tag,
	int,
	std::ptrdiff_t
> UIntAnyRange;


namespace PseudoRandomSequences {

	class BadArgumentException : public std::runtime_error {
	public:
		BadArgumentException() : std::runtime_error("Bad Argument Exception") {}
	};

	//no check on wrong value (2, 3 and etc.)
	//typedef uint32_t bool;
	
	//input: SequenceRandAccessContainer {0, 1} ^ n with P(1) = 0.1 (possibility of 1)
	//output: SequenceRandAccessContainer {0, 1} ^ n  with P(1) = 0.5

	//TODO: make template <type, int dimensionSize>, might be without first parameter
	//TODO: add matrix storage type into template parameter (array - stack storage data, vector - heap storage)
	template <class SequenceRandAccessContainer>
	class MatrixRandomGenerator {
	public:
		//matrixLength = 2 ^ dimension
		MatrixRandomGenerator(uint32_t dimension = 1) { setDimension(dimension); }
		MatrixRandomGenerator(const std::string& filename) { load(filename); }
		MatrixRandomGenerator(MatrixRandomGenerator&& other) { swap(*this, other); }
		
		void setDimension(uint32_t dimension1);
		uint32_t getDimension() const { return dimension; }

		//-------Selectors--------//

		SequenceRandAccessContainer converse(const SequenceRandAccessContainer &) const;
		void converse(SequenceRandAccessContainer &, const SequenceRandAccessContainer &) const;

		uint32_t rows() const { return rowCount; }

		void save(const std::string& filename, char separator = ' ');
		void load(const std::string& filename);

		const MatrixRandomGenerator& operator=(MatrixRandomGenerator other);
		template <class SequenceRandAccessContainer>
		friend void swap(MatrixRandomGenerator<SequenceRandAccessContainer>& first, 
			MatrixRandomGenerator<SequenceRandAccessContainer>& second);

		bool get(uint32_t i, uint32_t j) const { return matrix[i * 2 + j]; }
	private:
		//bool& rget(uint32_t i, uint32_t j) { return matrix[i * 2 + j]; }
		void set(uint32_t i, uint32_t j, bool value) { matrix[i * 2 + j] = value; }
		void setOnlyDimensionWithResizing(uint32_t dimension1);

	private:
		//matrix for conversation from SequenceRandAccessContainer first type into another one
		//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows
		std::vector<bool> matrix;
		uint32_t rowCount;
		uint32_t dimension;
		//Notice: if you add new variables don't forget about swap
	};

	template <class SequenceRandAccessContainer>
	inline SequenceRandAccessContainer MatrixRandomGenerator<SequenceRandAccessContainer>::converse(const SequenceRandAccessContainer & source) const {
		SequenceRandAccessContainer newSeq(source.size());
		converse(newSeq, source);
		return newSeq;
	}

	template <  class SequenceRandAccessContainer>
	inline void MatrixRandomGenerator<SequenceRandAccessContainer>::converse(SequenceRandAccessContainer & dest, const SequenceRandAccessContainer & source) const {
		//dest.resize(source);		//for just in case
		uint32_t row = 0;	//row number (not row count)
							//00...00

		for (size_t i = 0; i < source.size(); i++) {
			bool val = get(row, source[i]);
			dest[i] = val;
			//erase bit k (== dim)
			row &= ~(1 << (dimension - 1));		//what for??
												//shift left
			row <<= 1;
			//assign new bit
			row |= static_cast<uint32_t>(val);

			//Result: we change 'row' variable by shifting binary number to left 
			//			and writing into zero bit the value of dest[i]
		}
	}

	template <  class SequenceRandAccessContainer>
	void swap(MatrixRandomGenerator<SequenceRandAccessContainer>& first, 
					MatrixRandomGenerator<SequenceRandAccessContainer>& second) {
		using std::swap;
		swap(first.matrix, second.matrix);
		swap(first.rowCount, second.rowCount);
		swap(first.dimension, second.dimension);
	}

	//-------------------Configuring---------------------//

	template <  class SequenceRandAccessContainer>
	inline void MatrixRandomGenerator<SequenceRandAccessContainer>::setDimension(uint32_t dimension1) {

		setOnlyDimensionWithResizing(dimension1);

		//TODO: may be optimize the matrix refilling 
		//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows

		//1 0
		//0 1
		set(0, 0, 1);
		set(0, 1, 0);
		set(1, 0, 0);
		set(1, 1, 1);

		for (uint32_t degree = 2; degree < rowCount; degree *= 2) {
			for (uint32_t i = degree; i < 2 * degree; i++) {
				set(i, 0, get(i - degree, 1));		//matrix[i][0] = matrix[i - degree][1];
				set(i, 1, get(i - degree, 0));
			}
		}
	}

	template <  class SequenceRandAccessContainer>
	inline void MatrixRandomGenerator<SequenceRandAccessContainer>::setOnlyDimensionWithResizing(uint32_t dimension1)
	{
		if (dimension1 <= 0 || dimension1 > 32)		//depends on type uint32_t of row count
			throw BadArgumentException();

		dimension = dimension1;
		rowCount = 1 << dimension1;
		matrix.resize(2 * rowCount);
	}

	//TODO: test it
	template <  class SequenceRandAccessContainer>
	inline void MatrixRandomGenerator<SequenceRandAccessContainer>::load(const std::string& filename) {
		std::ifstream inFile;
		uint32_t dim1;

		inFile.open(filename, ios::in);

		inFile >> dim1;
		setOnlyDimensionWithResizing(dim1);

		//TODO: where exceptions????? Wrong file
		std::copy(std::istream_iterator<bool>(inFile),
			std::istream_iterator<bool>(), matrix.begin());
		/*for (uint32_t i = 0; i < rowCount; i++) {
		bool elem1,
		elem2;
		inFile >> elem1;
		set(i, 0, elem1);
		inFile >> elem2;
		set(i, 1, elem2);
		}*/

		inFile.close();
	}

	template <  class SequenceRandAccessContainer>
	inline void MatrixRandomGenerator<SequenceRandAccessContainer>::save(const std::string& filename, char separator) {
		ofstream outFile;

		outFile.open(filename, ios::out | ios::trunc);
		outFile << dimension << std::endl;

		//Try to storage binary

		//In memory we will storage only vector
		std::copy<bool>(matrix.cbegin(), matrix.cend(), 
			std::ostream_iterator<bool>(outFile, " "));

		outFile.close();
	}

	template <  class SequenceRandAccessContainer>
	inline const MatrixRandomGenerator<SequenceRandAccessContainer> & MatrixRandomGenerator<SequenceRandAccessContainer>::operator=(MatrixRandomGenerator<SequenceRandAccessContainer> other)
	{
		swap(*this, other);
		return *this;
	}

	template <  class SequenceRandAccessContainer>
	inline std::ostream& operator<< (std::ostream & o, const SequenceRandAccessContainer & seq) {
		for (size_t i = 0; i < seq.size(); i++) {
			o << seq[i] << " ";
		}
		return o;
	}

	//-------Print------------//

	void printBits(std::ostream & o, uint32_t bits, uint32_t count = 32) {
		for (uint32_t i = 1 << (count - 1); i > 0; i >>= 1)
			o << (bits & i);
	}
	
}

#endif //SEQUENCE_GENERATOR_H
