
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

const int TEST_COUNT = 16;

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

	int len = -1;
	if (argc < 5 || (len = std::strlen(argv[3])) < TEST_COUNT) {
		cout << "Not enough parameters ( matrix dimension, sequence size, testKey ("
			<< TEST_COUNT << ", current = " << len << "), input possibility, "
			<< "file input, file output)" 
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	size_t inputSize = size_t(boost::lexical_cast<double>(argv[2]));
	Sequence seq;
	//Sequence seq(size_t(boost::lexical_cast<double>(argv[2])));
	// TODO: bad computation of input possibility (use another input format)
	//std::default_random_engine generator;
	std::random_device generator;
	//std::ranlux48 generator;				//failure with normal_distribution and with chi_squared_distribution
	std::normal_distribution<double> distribution(5.0, 2.0);		//doesn't failure with random_device generator
	//std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device and with number of freedoms = 3.0
	int inputOppositePossibility = static_cast<int>(
		std::round(1.0 / boost::lexical_cast<double>(argv[4]))
	);
	//std::generate_n(std::back_inserter(seq), inputSize,
	//	[&inputOppositePossibility, &generator, &distribution, &inputSize]() -> bool {
	//		//return (rand() % inputOppositePossibility == 0);
	//	return (int(std::round(distribution(generator))) % inputOppositePossibility == 0);
	//});

				//-------------Input----------------//
	std::ifstream inFile;
	inFile.open(argv[5], std::ios::in);
	inFile >> std::skipws;
	auto iter = std::istream_iterator<char>(inFile);
	for (size_t i = 0; i < inputSize && iter != std::istream_iterator<char>(); i++, iter++) {
		// bool per char
		//seq.push_back( (*iter) == '1' );
		// bool per bit
		for (int j = 0; j < 8; j++) {
			seq.push_back((*iter) & (1 << j));
		}
	}
	inFile.close();

	Sequence result;
	uint32_t dimension = uint32_t(boost::lexical_cast<double>(argv[1]));
	//MatrixRandomConverter<> converter(dimension);
	//result = converter.converse(seq);
	result = std::move(seq);
	cout << endl << "Seq size = " << result.size() << endl;

				//-------------Output----------------//

	std::ofstream outFile;
	outFile.open(argv[6], std::ios::out | std::ios::trunc);
	//std::copy(result.begin(), result.end(), std::ostream_iterator<bool>(outFile, ""));
	auto outIter = std::ostream_iterator<uint32_t>(outFile);
	int i = 0;
	uint32_t buffer = 0u;
	for (auto iter : result) {
		buffer |= iter << (i++);
		if (i >= 32) {
			i = 0;
			*(outIter++) = buffer;
		}
	}
	outFile.close();

	if (result.size() == 0)
	{
		cout << "empty sequence " << argv[5] << endl;
		return -1;
	}
	
	string testKey(argv[3]);
	epsilon = std::move(result);
	
	if (testKey[0] == '1') {
		// ! Each bit means 0 or 1 (you can't pass to bookStackTest 0 or 1 in whole byte for example)

		std::string sizeStr = std::to_string(inputSize);
		std::string dimStr = std::to_string(dimension);
		std::vector<const char *> arguments{ "bs.exe",
			"-f", argv[5],
			"-n", sizeStr.c_str(),	// file size (in bits)
			"-w", dimStr.c_str(),				// word size (or alphabet symbol length (see yourself book stack version)
			//"-b", "0",				// blank between words
			//"-u", "32"				// size of upper part book stack
		};
		double chi = bookStackTestMain(int(arguments.size()), &arguments[0]);
		double p_value = 1 - 
			boost::math::cdf(boost::math::chi_squared_distribution<double>(1), chi);

		cout << "BookStack stat:\t\t\t" << ((p_value > 0.01) ? "SUCCESS" : "FAILURE") 
			<< "\t\tp_value = " << p_value << endl << endl;
	}
	if (testKey[1] == '1') {
		Frequency(epsilon.size());
	}
	if (testKey[2] == '1') {
		BlockFrequency(dimension, epsilon.size());		//doesn't equal frequency monobit with M = 1
	}
	if (testKey[3] == '1') {
		Runs(epsilon.size());
	}
	if (testKey[4] == '1') {
		LongestRunOfOnes(epsilon.size());
	}
	if (testKey[5] == '1') {
		Rank(epsilon.size());
	}
	if (testKey[6] == '1') {
		// Has a little difference between results of my own discreteFourier Test version
		DiscreteFourierTransform(epsilon.size());
	}
	if (testKey[7] == '1') {
			// from 2 to 16
		NonOverlappingTemplateMatchings(5, epsilon.size());
	}
	if (testKey[8] == '1') {
		OverlappingTemplateMatchings(5, epsilon.size());
	}
	if (testKey[9] == '1') {
		Universal(epsilon.size());
	}
	if (testKey[10] == '1') {
		LinearComplexity(1 << dimension, epsilon.size());
	}
	if (testKey[11] == '1') {
		Serial(dimension, epsilon.size());
	}
	if (testKey[12] == '1') {
		// (M + 1) - bit block is used to compare
		ApproximateEntropy(dimension, epsilon.size());
	}
	if (testKey[13] == '1') {
		CumulativeSums(epsilon.size());
	}
	if (testKey[14] == '1') {
		RandomExcursions(epsilon.size());
	}
	if (testKey[15] == '1') {		// For more longer sequences (> 1e6)
		RandomExcursionsVariant(epsilon.size());
	}

	return 0;
}

