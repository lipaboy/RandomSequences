#include "SequenceConverter.h"

#include <fstream>

namespace PseudoRandomSequences {

	bool bookPileTest(const Sequence & seq)
	{
		const uint32_t alphabetSize = 2;
		//map: alphabet -> pile position
		vector<uint32_t> pile(alphabetSize);
		const uint32_t plentyCounts = alphabetSize;
		//map: { {0}, {1} } -> frequency of meeting symbol into {0} or {1} before
		//moving this symbol to pile peek
		vector<uint32_t> freq(plentyCounts);

		for (uint32_t i = 0; i < pile.size(); i++) {
			pile[i] = i;
			freq[i] = 0;
		}

		for (uint32_t t = 1; t <= seq.size(); t++) {
			const uint32_t currSymbol = seq[t - 1];
			const uint32_t prevPos = pile[currSymbol];

			//old pile state (t - 1)
			if (prevPos == currSymbol)
				freq[currSymbol]++;

			pile[currSymbol] = 0;		//move to pile peek (up)
			for (uint32_t i = 0; i < pile.size(); i++) {
				if (pile[i] < prevPos)
					++pile[i];				//move down
			}

			//new pile state (t)
		}

		//Meaning: symbols with equal possibility can be (turn out to be) on any pile position

		double statisticX2 = 0;
		const uint32_t N = seq.size();
		for (uint32_t j = 0; j < freq.size(); j++) {
			statisticX2 += std::pow((freq[j] * 1.0 / (N / 2.0) - 1), 2);
		}

		std::cout << "StatisticX^2 = " << statisticX2 << endl;
		statisticX2 *= N / 2.0;
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