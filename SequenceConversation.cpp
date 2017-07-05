#include "SequenceConversation.h"

#include <fstream>

namespace PseudoRandomSequences {
	SequenceConverter::SequenceConverter(uint32 matrixLength) : dimension(matrixLength) {
		if (matrixLength == 0)
			throw BadArgumentException();

		//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows
		resize(matrixLength);
		//1 0
		//0 1
		set(0, 0, 1);
		set(0, 1, 0);
		set(1, 0, 0);
		set(1, 1, 1);

		for (uint32 degree = 2; degree < rowCount; degree *= 2) {
			for (uint32 i = degree; i < 2 * degree; i++) {
				set(i, 0, get(i - degree, 1));		//matrix[i][0] = matrix[i - degree][1];
				set(i, 1, get(i - degree, 0));
//				matrix[i * rowCount + 0] = matrix[(i - degree) * rowCount + 1];
			}
		}
	}

	SequenceConverter::SequenceConverter(const string& filename) {
		load(filename);
	}


	Sequence SequenceConverter::converse(const Sequence& seq) const {
		Sequence newSeq(seq.size());
		uint32 rowNum = 0;	//row number (not row count)
					//00...00

		for (size_t i = 0; i < seq.size(); i++) {
			newSeq[i] = get(rowNum, seq[i]);
			//erase bit k
			rowNum &= ~(1 << (dimension - 1));
			//shift left
			rowNum <<= 1;
			//assign new bit
			if (newSeq[i] == 1)
				rowNum |= 1;
			//printBits(cout, rowNum);
			//cout << " ";
		}

		return newSeq;
	}


	ostream& operator<< (ostream & o, const SequenceConverter& seq) {
		for (uint32 j = 0; j < seq.rows(); j++) {
			for (uint32 i = 0; i < seq.columns(); i++) {
				o << seq.get(j, i) << " ";
			}
			o << std::endl;
		}
		return o;
	}

	void SequenceConverter::resize(uint32 dimension1) {
		if (dimension > 0) {
			dimension = dimension1;
			rowCount = 1 << dimension1;
			matrix.resize(2 * rowCount);
		}
	}

	void SequenceConverter::load(const string& filename) {
		ifstream inFile;

		inFile.open(filename, ios::in);

		inFile >> dimension;
		resize(dimension);
		for (uint32 i = 0; i < rowCount; i++) {
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

	void printBits(ostream& o, uint32 bits, uint32 count) {
		for (uint32 i = 1 << (count - 1); i > 0; i >>= 1) 
			o << (bits & i);
	}
}