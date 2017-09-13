
#include "book_stack_test.h"
#include "discrete_fourier_transform_test.h"
#include "sequence_converter.h"
#include "statisticChiSquared.h"

#include <iterator>
#include <bitset>
#include <string>
#include <array>

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>

using namespace PseudoRandomSequences;
using namespace std::chrono;

//TODO: try to use GoogleTests

typedef std::vector<bool> Sequence;
//typedef MatrixRandomConverter<Sequence> VectorMatrixRandomGenerator;

int main(int argc, char *argv[]) {
	time_t t;
	srand((unsigned)time(&t));
	using std::string;
	using std::vector;
	using std::cout;
	using std::endl;

	if (argc < 3) {
		cout << "Not enough parameters (dimension, sequence size)" << endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	uint32_t dimension = uint32_t(boost::lexical_cast<double>(argv[1]));
	MatrixRandomConverter<> converter(dimension);

	// Test Fourier

	steady_clock::time_point start = steady_clock::now();

	Sequence seq(size_t(boost::lexical_cast<double>(argv[2])));

	// 1 Test
	//std::fill(seq.begin(), seq.end(), 0);
	// 2 Test
	std::generate(seq.begin(), seq.end(), 
		[] () -> bool { 
		static size_t i = 0;
		return 0;
	});

	cout << "Fourier stat = " << discreteFourierTransformTest(seq) 
		<< " Time: " << (steady_clock::now() - start).count() / 1e3 << endl;
	

	return 0;
}

