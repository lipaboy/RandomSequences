#include "sequence_converter.h"

#include <fstream>
#include <algorithm>
#include <iterator>

//#include "isequence.h"

namespace PseudoRandomSequences {
	
	bool bookStackTest(const Sequence & seq) {

		const uint32_t alphabetSize = 2;
		//map: alphabet -> stack position
		vector<uint32_t> stack(alphabetSize);
		const uint32_t plentyCounts = alphabetSize;
		//map: { stack position } -> frequency of meeting symbol onto this position
		//moving this symbol to stack peek
		vector<uint32_t> freq(plentyCounts);

		for (uint32_t i = 0; i < stack.size(); i++) {
			stack[i] = i;
			freq[i] = 0;
		}

		for (uint32_t t = 1; t <= seq.size(); t++) {
			const uint32_t currSymbol = seq[t - 1];
			const uint32_t prevPos = stack[currSymbol];

			//old stack state (t - 1)
							//if (prevPos == currSymbol)	--wrong
			freq[prevPos]++;

			//std::cout << "(" << stack[0] << ", " << stack[1] << ") ";
			if (prevPos > 0)		//not necessary
				for (uint32_t i = 0; i < stack.size(); i++)
					if (stack[i] < prevPos)
						++stack[i];				//move down
			stack[currSymbol] = 0;		//move to stack peek (up)

			//new stack state (t)
		}
		//std::cout << std::endl << freq[0] << " " << freq[1] << std::endl;

		//Meaning: symbols with equal possibility can be (turn out to be) on any stack position

		//TODO: replace (N / 2) on (N / alphabetSize)

		double statisticX2 = 0;
		const uint32_t N = seq.size();
		for (uint32_t j = 0; j < freq.size(); j++) {
			statisticX2 += std::pow(freq[j] - (N / 2.0), 2) / (N / 2.0);
		}

		double statisticX2Correction = 0;	//Yates's correction
		for (uint32_t j = 0; j < freq.size(); j++) {
			statisticX2Correction += std::pow(std::abs(freq[j] - (N / 2.0)) - 0.5, 2) / (N / 2.0);
		}

		std::cout << "StatisticX^2 = " << statisticX2 << endl;
		std::cout << "StatisticX^2 with Yates's correction = " << statisticX2Correction << endl;
		//statisticX2 *= N / 2.0;
		//std::cout << "StatisticX^2 = " << statisticX2 << endl;

		return false;
	}

	Sequence SequenceConverter::converse(const Sequence& seq) const {
		Sequence newSeq(seq.size());
		uint32_t row = 0;	//row number (not row count)
					//00...00

		for (size_t i = 0; i < seq.size(); i++) {
			AlphabetType val = get(row, seq[i]);
			newSeq[i] = val;
			//erase bit k (== dim)
			row &= ~(1 << (dimension - 1));		//what for??
			//shift left
			row <<= 1;
			//assign new bit
			row |= val;

			//Result: we change 'row' variable by shifting binary number to left 
			//			and writing into zero bit the value of newSeq[i]
		}

		return newSeq;
	}

	//-------------------Configuring---------------------//

	ostream& operator<< (ostream & o, const SequenceConverter& seq) {
		for (uint32_t j = 0; j < seq.rows(); j++) {
			for (uint32_t i = 0; i < seq.columns(); i++) {
				o << seq.get(j, i) << " ";
			}
			o << std::endl;
		}
		return o;
	}

	void SequenceConverter::setDimension(uint32_t dimension1) {
		const uint32_t oldDim = dimension;

		setOnlyDimensionWithResizing(dimension1);

		//TODO: may be optimize the matrix refilling 
		if (dimension1 > oldDim) {
			//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows

			//1 0
			//0 1
			set(0, 0, 1);
			set(0, 1, 0);
			set(1, 0, 0);
			set(1, 1, 1);

			for (uint32_t degree = 2; degree < rowCount; degree *= 2) {
				for (uint32_t i = degree; i < 2 * degree; i++) {
					rget(i, 0) = get(i - degree, 1);		//matrix[i][0] = matrix[i - degree][1];
					rget(i, 1) = get(i - degree, 0);
				}
			}
		}
	}

	void SequenceConverter::setOnlyDimensionWithResizing(uint32_t dimension1)
	{
		if (dimension1 <= 0 && dimension1 > 32)		//depends on type uint32_t of row count
			throw BadArgumentException();

		dimension = dimension1;
		rowCount = 1 << dimension1;
		matrix.resize(2 * rowCount);
	}

	//TODO: change writing to file (write matrix in row to economy symbols count in file)
	void SequenceConverter::load(const string& filename) {
		ifstream inFile;
		uint32_t dim1;

		inFile.open(filename, ios::in);

		inFile >> dim1;
		setOnlyDimensionWithResizing(dim1);
		for (uint32_t i = 0; i < rowCount; i++) {
			AlphabetType elem1,
				elem2;
			inFile >> elem1;
			set(i, 0, elem1);
			inFile >> elem2;
			set(i, 1, elem2);
		}

		inFile.close();
	}

	void SequenceConverter::save(const string& filename, char separator) {
		ofstream outFile;

		outFile.open(filename, ios::out | ios::trunc);
		outFile << dimension << std::endl;
		outFile << (*this);
		outFile.close();
	}

	ostream& operator<< (ostream & o, const Sequence& seq) {
		for (size_t i = 0; i < seq.size(); i++) {
			o << seq[i] << " ";
		}
		return o;
	}

	void printBits(ostream& o, uint32_t bits, uint32_t count) {
		for (uint32_t i = 1 << (count - 1); i > 0; i >>= 1) 
			o << (bits & i);
	}
}