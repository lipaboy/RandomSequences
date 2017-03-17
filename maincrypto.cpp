#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "SequenceConversation.h"

using namespace std;
using namespace PseudoRandomSequences;

//TODO: try to use GoogleTests
int main(void) {
	time_t t;
	srand((unsigned)time(&t));



	//string filename = "BinaryMatrixGenerator.txt";
	const uint32 k = 3;
	SequenceConversation Matrix(k);
	const uint32 N = 30;
	Sequence seq(N);

	//possibility(1) = 1 / 10
	for (size_t i = 0; i < seq.size(); i++) {
		seq[i] = (0 == std::rand() % 10) ? 1 : 0;
	}
	cout << seq << endl;
	cout << Matrix.converse(seq) << endl;


	return 0;
}

