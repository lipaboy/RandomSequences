#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "SequenceConverter.h"

using namespace std;
using namespace PseudoRandomSequences;

//TODO: try to use GoogleTests
int main(void) {
	time_t t;
	srand((unsigned)time(&t));

	//string filename = "BinaryMatrixGenerator.txt";

	uint32_t k = 8;
	SequenceConverter converter(k);

	for (; k <= 8; k++) {
		converter.setDimension(k);
		const uint32_t N = static_cast<uint32_t>(1e5);
		Sequence seq(N);

		//possibility(1) = 1 / 10
		for (size_t i = 0; i < seq.size(); i++) {
			seq[i] = (0 == std::rand() % 10) ? 1 : 0;
		}
		//cout << seq << endl;
		Sequence newSeq = converter.converse(seq);
		//cout << newSeq << endl;

		bookPileTest(newSeq);
	}


	return 0;
}

