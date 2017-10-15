
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

#include "statTests\include\stat_fncs.h"

using namespace PseudoRandomSequences;
using namespace std::chrono;

//TODO: try to use GoogleTests

typedef std::vector<bool> Sequence;
//typedef MatrixRandomConverter<Sequence> VectorMatrixRandomGenerator;

#include <boost/math/distributions/inverse_chi_squared.hpp>

int main(int argc, char *argv[]) {
	time_t t;
	std::srand(unsigned int(std::time(&t)));
	using std::string;
	using std::vector;
	using std::cout;
	using std::endl;

	//double possibility = boost::math::cdf(boost::math::chi_squared_distribution<double>(n), statistic);
	//cout << "Alpha = " << possibility << endl;
	///*double chi2 = boost::math::pdf(boost::math::inverse_chi_squared_distribution<double>(n), alpha);
	//cout << "Chi2 = " << chi2 << endl;*/

	if (argc < 5 || std::strlen(argv[3]) < 5) {
		cout << "Not enough parameters ( matrix dimension, sequence size, testKey, input possibility)" 
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	uint32_t dimension = uint32_t(boost::lexical_cast<double>(argv[1]));
	MatrixRandomConverter<> converter(dimension);

	size_t inputSize = size_t(boost::lexical_cast<double>(argv[2]));
	Sequence seq;
	//Sequence seq(size_t(boost::lexical_cast<double>(argv[2])));
	// TODO: bad computation of input possibility (use another input format)
	std::default_random_engine generator;
	//std::random_device generator;
	//std::ranlux48 generator;				//failure with normal_distribution and with chi_squared_distribution
	std::normal_distribution<double> distribution(5.0, 2.0);		//doesn't failure with random_device generator
	//std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device
	int inputOppositePossibility = static_cast<int>(
		std::round(1.0 / boost::lexical_cast<double>(argv[4]))
	);
	std::generate_n(std::back_inserter(seq), inputSize,
		[&inputOppositePossibility, &generator, &distribution, &inputSize]() -> bool {
			//return (rand() % inputOppositePossibility == 0);
		return (int(std::round(distribution(generator))) % inputOppositePossibility == 0);
	});

				//-------------Input----------------//
	/*std::ifstream inFile;
	inFile.open(argv[5], std::ios::in);
	inFile >> std::skipws;
	auto iter = std::istream_iterator<char>(inFile);
	for (size_t i = 0; i < inputSize && iter != std::istream_iterator<char>(); i++, iter++) {
		seq.push_back( (*iter) == '1' );
	}
	inFile.close();*/

	Sequence result;
	//steady_clock::time_point start = steady_clock::now();
	volatile clock_t start = clock();
	//result = converter.converse(seq);
	result = std::move(seq);
	cout << " Conversation time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.)
		//<< " rows = " << converter.rows() 
		<< endl << "Seq size = " << result.size() << endl;

				//-------------Output----------------//
	/*std::ofstream outFile;
	outFile.open(argv[5], std::ios::out | std::ios::trunc);
	std::copy(result.begin(), result.end(), std::ostream_iterator<bool>(outFile, ""));
	outFile.close();*/

	if (result.size() == 0)
	{
		cout << "empty sequence " << argv[5] << endl;
		return 0;
	}
	
	string testKey(argv[3]);
	
	if (testKey[0] == '1') {
		start = clock();
		cout << "BookStack stat = ";// << bookStackTest(result, dimension) << endl;
		
		std::vector<const char *> arguments{ "bs.exe",
			"-f", argv[5],
			"-n", itoa(INT_MAX, nullptr, 10),
			"-w", "8",
			"-u", "32"
		};
		std::copy(arguments.begin(), arguments.end(), std::ostream_iterator<const char *>(cout, " "));
		double chi = bookStackTestMain(arguments.size(), &arguments[0]);
		double p_value = 1 - 
			boost::math::cdf(boost::math::chi_squared_distribution<double>(1), chi);

		cout << ((p_value > 0.01) ? "SUCCESS" : "FAILURE") << "\tp_value = " << p_value << endl;
		cout << " Time: " << (clock() - start) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	if (testKey[1] == '1') {
		start = clock();
		cout << "Fourier stat = "; 
		DiscreteFourierTransform(result.size(), result);
		//cout << discreteFourierTransformTest(result) << endl;
		cout << " Time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	std::swap(epsilon, result);
	if (testKey[2] == '1') {
		start = clock();
		//cout << "ChiSquared = " << statisticChiSquaredTest(result, dimension) << endl;
		cout << "Frequency (Monobit) = ";
		Frequency(epsilon.size());
		cout << " Time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	if (testKey[3] == '1') {
		start = clock();
		cout << "Runs (depends on Frequency Monobit) stat = ";
		Runs(epsilon.size());
		cout << " Time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
	}
	// Warning: result haven't already contained current sequence
	if (testKey[4] == '1') {
		start = clock();
		cout << "Longest runs of ones stat = ";
		LongestRunOfOnes(epsilon.size());
		cout << " Time: " << (clock() - start + 0.) / (CLOCKS_PER_SEC / 1000.) << endl;
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

