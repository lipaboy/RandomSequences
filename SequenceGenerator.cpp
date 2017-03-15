#include "SequenceGenerator.h"

#include <fstream>

namespace PseudoRandomSequences {
	SequenceGenerator::SequenceGenerator(uint32 k) {		//k - matrix dimension
		if (k == 0)
			throw BadArgumentException();

		rowCount = 1 << k;	//std::pow(2, k)
		//<0, 1> - columns, <00..00, 00..01, ..., 11..10, 11..11> - rows
		matrix.resize(columnCount * rowCount);
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

	ostream& operator<< (ostream & o, const SequenceGenerator& seq) {
		for (uint32 j = 0; j < seq.rows(); j++) {
			for (uint32 i = 0; i < seq.columns(); i++) {
				o << seq.get(j, i) << " ";
			}
			o << std::endl;
		}
		return o;
	}


	void SequenceGenerator::save(const string& filename, char separator) {
		ofstream outFile;
		
		outFile.open(filename, ios::out | ios::trunc);

		outFile << (*this);

		outFile.close();

	}


}