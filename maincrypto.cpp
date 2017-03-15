#include <iostream>

#include "SequenceGenerator.h"

using namespace std;
using namespace PseudoRandomSequences;

//TODO: try to use GoogleTests
int main(void) {
	SequenceGenerator seq(3);

	std::cout << seq << std::endl;
	//std::cout << seq.get(0, 0) << std::endl;
	seq.save("BinaryMatrixGenerator.txt");

	return 0;
}

