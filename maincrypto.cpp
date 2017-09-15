
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

	if (argc < 4 || std::strlen(argv[3]) < 3) {
		cout << "Not enough parameters (dimension, sequence size)" << endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	uint32_t dimension = uint32_t(boost::lexical_cast<double>(argv[1]));
	MatrixRandomConverter<> converter(dimension);

	Sequence seq(size_t(boost::lexical_cast<double>(argv[2])));
	std::generate(seq.begin(), seq.end(), 
		[]() -> bool {
			return (rand() % 10 == 0);
	});

	Sequence result(seq.size());
	//steady_clock::time_point start = steady_clock::now();
	volatile clock_t start = clock();
	converter.converse(result, seq);
	cout << " Conversation time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
	
	string testKey(argv[3]);
	if (testKey[0] == '1') {
		start = clock();
		cout << "ChiSquared = " << statisticChiSquaredTest(result, dimension) << endl
			<< " Time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	if (testKey[1] == '1') {
		start = clock();
		cout << "BookStack stat = " << bookStackTest(result, dimension) << endl
			<< " Time: " << (clock() - start) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	if (testKey[2] == '1') {
		start = clock();
		cout << "Fourier stat = " << discreteFourierTransformTest(result) << endl
			<< " Time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	

	//-------------------Test Fourier---------------------//

	// 1 Test (0^n)
	//std::fill(seq.begin(), seq.end(), 0);
	// 2 Test (0011)^n
	/*std::generate(seq.begin(), seq.end(), 
		[] () -> bool { 
		static size_t i = 0;
		return ((i++) % 4 < 2);
	});*/
	// 3 Test (std::rand)
	/*std::generate(seq.begin(), seq.end(),
		[]() -> bool {
		return rand() % 2;
	});*/
	// 4 Test
	/*std::generate(seq.begin(), seq.end(),
		[&seq]() -> bool {
		static size_t i = 0;
		return ((i++) % (seq.size() / 4) < seq.size() / 8);
	});*/

	// 5 Test	(110100)^n
	/*std::generate(seq.begin(), seq.end(),
		[&seq]() -> bool {
		static size_t i = 0;
		size_t j = (i++) % 6;
		return (j < 2 || j == 3);
	});*/

	// 6 Test	(1110011000)^n
	//std::generate(seq.begin(), seq.end(),
	//	[&seq]() -> bool {
	//	static size_t i = 0;
	//	size_t j = (i++) % 10;
	//	return (j < 3 || j == 5 || j == 6);
	//});

	// 7 Test	(1 rand() 0)^n
	/*std::generate(seq.begin(), seq.end(),
		[&seq]() -> bool {
		static size_t i = 0;
		size_t j = (i++) % 3;
		return ((j == 1) ? rand() % 2 : ((j < 1) ? 1 : 0));
	});*/

	

	return 0;
}

