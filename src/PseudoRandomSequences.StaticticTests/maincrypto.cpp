
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

	size_t len = 0;
	if (argc < 7 || (len = std::strlen(argv[3])) < TEST_COUNT) {
		cout << "Not enough parameters ( matrix dimension, sequence size, testKey ("
			<< TEST_COUNT << ", current = " << len << "), input possibility, "
			<< "input file, output file, result output file)" 
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	// Kbits
	size_t inputSize = 1024u * size_t(boost::lexical_cast<double>(argv[2]));
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
	//inFile >> std::skipws;
	auto iter = std::istream_iterator<char>(inFile);
	for (size_t i = 0; i < inputSize / 8
			//&& iter != std::istream_iterator<char>()
			; i++, iter++) {
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
	auto outIter = std::ostream_iterator<char>(outFile);
	int i = 0;
	char buffer = 0;
	for (auto val : result) {
		buffer |= val << (i++);
		if (i >= 8) {
			i = 0;
			*(outIter++) = buffer;
			buffer = 0;
		}
	}
	if (i > 0)
		*(outIter) = buffer;
	outFile.close();

	if (result.size() == 0)
	{
		cout << "empty sequence " << argv[5] << endl;
		return -1;
	}
	
	string testKey(argv[3]);
	epsilon = std::move(result);
	const int EPSILON_SIZE = int(epsilon.size());
	
	vector<string> testNames;
	vector<double> testResults;

	if (testKey[0] == '1') {
		// ! Each bit means 0 or 1 (you can't pass to bookStackTest 0 or 1 in whole byte for example)
		std::string sizeStr = std::to_string(EPSILON_SIZE);
		std::string dimStr = std::to_string(dimension);
		std::vector<const char *> arguments{ "bs.exe",
			"-f", argv[6],
			"-n", sizeStr.c_str(),	// file size (in bits)
			"-w", dimStr.c_str(),				// word size (or alphabet symbol length (see yourself book stack version)
			//"-b", "0",				// blank between words
			//"-u", "32"				// size of upper part book stack
		};
		testNames.push_back("BookStackTest_" + dimStr);
		testResults.push_back(bookStackTestMain(int(arguments.size()), &arguments[0]));
	}
	if (testKey[1] == '1') {
		testNames.push_back("Frequency");
		testResults.push_back(Frequency(EPSILON_SIZE));
	}
	if (testKey[2] == '1') {
		//doesn't equal frequency monobit with M = 1
		testNames.push_back("BlockFrequency_" + std::to_string(dimension));
		testResults.push_back(BlockFrequency(dimension, EPSILON_SIZE));
	}
	if (testKey[3] == '1') {
		testNames.push_back("Runs");
		testResults.push_back(Runs(EPSILON_SIZE));
	}
	if (testKey[4] == '1') {
		testNames.push_back("LongestRunOfOnes");
		testResults.push_back(LongestRunOfOnes(EPSILON_SIZE));
	}
	if (testKey[5] == '1') {
		testNames.push_back("Rank");
		testResults.push_back(Rank(EPSILON_SIZE));
	}
	if (testKey[6] == '1') {
		// Has a little difference between results of my own discreteFourier Test version
		testNames.push_back("DiscreteFourierTransform");
		testResults.push_back(DiscreteFourierTransform(EPSILON_SIZE));
	}
	if (testKey[7] == '1') {
			// from 2 to 16
		testNames.push_back("NonOverlappingTemplateMatchings_" + std::to_string(5));
		testResults.push_back(NonOverlappingTemplateMatchings(5, EPSILON_SIZE));
	}
	if (testKey[8] == '1') {
		testNames.push_back("OverlappingTemplateMatchings_" + std::to_string(5));
		testResults.push_back(OverlappingTemplateMatchings(5, EPSILON_SIZE));
	}
	if (testKey[9] == '1') {
		testNames.push_back("Universal");
		testResults.push_back(Universal(EPSILON_SIZE));
	}
	if (testKey[10] == '1') {
		testNames.push_back("LinearComplexity_" + std::to_string(1 << dimension));
		testResults.push_back(LinearComplexity(1 << dimension, EPSILON_SIZE));
	}
	if (testKey[11] == '1') {
		testNames.push_back("Serial_" + std::to_string(dimension));
		testResults.push_back(Serial(dimension, EPSILON_SIZE));
	}
	if (testKey[12] == '1') {
		testNames.push_back("ApproximateEntropy_" + std::to_string(dimension));
		// (M + 1) - bit block is used to compare
		testResults.push_back(ApproximateEntropy(dimension, EPSILON_SIZE));
	}
	if (testKey[13] == '1') {
		testNames.push_back("CumulativeSums");
		testResults.push_back(CumulativeSums(EPSILON_SIZE));
	}
	if (testKey[14] == '1') {
		testNames.push_back("RandomExcursions");
		testResults.push_back(RandomExcursions(EPSILON_SIZE));
	}
	if (testKey[15] == '1') {		// For more longer sequences (> 1e6)
		testNames.push_back("RandomExcursionsVariant");
		testResults.push_back(RandomExcursionsVariant(EPSILON_SIZE));
	}

	std::ofstream resFile;
	resFile.open(argv[7], std::ios::out | std::ios::trunc);

	std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
	resFile << endl;
	std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"));
	resFile << endl;

	resFile.close();

	return 0;
}

