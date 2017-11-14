#include "pseudoRandomSequences.h"

#include <iterator>
#include <bitset>
#include <string>
#include <array>
#include <cmath>
#include <random>

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>

#include "statTests\include\stat_fncs.h"
#include "lipaboyLibrary\src\maths\fixed_precision_number.h"

using namespace PseudoRandomSequences;
using namespace std::chrono;

//TODO: try to use GoogleTests

typedef std::vector<bool> Sequence;

const int TEST_COUNT = 16;

int PseudoRandomSequences::generatorsTestConfigRun(int argc, char * argv[]) {
	time_t t;
	std::srand(unsigned int(std::time(&t)));
	using std::string;
	using std::vector;
	using std::cout;
	using std::endl;


	size_t len = 0;
	if (argc < 3 || (len = std::strlen(argv[1])) < TEST_COUNT) {
		cout << "Not enough parameters ( testKey ("
			<< TEST_COUNT << ", current = " << len << "), "
			<< "input possibility, sequence size)"
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	std::ofstream resFile;
	std::ofstream extraFile;
	resFile.open("resStdGenerators.dat",
		std::ios::out | std::ios::trunc);
	extraFile.open("extraStdGenerators.dat",
		std::ios::out | std::ios::trunc);

	std::string inputFilename = "input";
	// Kbits
	size_t inputSize = 1024u * size_t(boost::lexical_cast<double>(argv[3]));

	std::vector<string> generatorNames{ "minstd_rand", 
		"default_random_engine", "ranlux48", "random_device" };

	for (int i = 0; i < generatorNames.size(); i++) {
		std::string & genName = generatorNames[i];

		//-------------Input----------------//

		Sequence seq;
		// TODO: bad computation of input possibility (use another input format) (bad because 2/3)

		std::minstd_rand generator;
		//std::default_random_engine generator;
		//std::ranlux48 generator;				//failure with normal_distribution and with chi_squared_distribution
		//std::random_device generator;

		std::normal_distribution<double> distribution(5.0, 2.0);		//doesn't failure with random_device generator
		//std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0)
		int inputOppositePossibility = static_cast<int>(
			std::round(1.0 / boost::lexical_cast<double>(argv[2]))
			);

		if (get)
		std::generate_n(std::back_inserter(seq), inputSize,
			[&inputOppositePossibility, &generator, &distribution, &inputSize]() -> bool {
			return (int(std::round(distribution(generator))) % inputOppositePossibility == 0);
		});

		Sequence result;
		uint32_t dimension = uint32_t(boost::lexical_cast<double>(argv[1]));
		//MatrixRandomConverter<> converter(dimension);
		//result = converter.converse(seq);
		result = std::move(seq);
		cout << endl << "Seq size = " << result.size() << endl;

		//-------------Output----------------//
		std::string outFilename = "out";
		{
			std::ofstream outFile;
			outFile.open(outFilename + std::to_string(fileIndex) + ".tmp", std::ios::out | std::ios::trunc);
			//std::copy(result.begin(), result.end(), std::ostream_iterator<bool>(outFile, ""));
			auto outIter = std::ostream_iterator<char>(outFile);
			int bitPos = 0;
			char buffer = 0;
			for (auto val : result) {
				buffer |= val << (bitPos++);
				if (bitPos >= 8) {
					bitPos = 0;
					*(outIter++) = buffer;
					buffer = 0;
				}
			}
			if (bitPos > 0)
				*(outIter) = buffer;
			outFile.close();
		}

		string testKey(argv[1]);
		epsilon = std::move(result);
		const int EPSILON_SIZE = int(epsilon.size());

		vector<string> testNames = { "" };
		vector<double> testResults;

		runTests(epsilon, testNames, testResults, testKey, fileIndex, outFilename);

		//----------------Write results-----------------//
		{
			if (fileIndex <= firstId) {
				std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
				resFile << endl;
			}
			resFile << fileIndex << "_Kbits\t";
			std::transform(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"),
				[](double p_value) -> double {
				return LipaboyLib::FixedPrecisionNumber<double, int, 1, -5>(p_value) == -1. ? -1. :
					p_value >= ALPHA;
			});
			resFile << endl;
		}
		//----------------Extra infos-----------------//
		{
			if (fileIndex <= firstId) {
				std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(extraFile, "\t"));
				extraFile << endl;
			}
			extraFile << fileIndex << "_Kbits\t";
			std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(extraFile, "\t"));
			extraFile << endl;
		}

		resFile.flush();
		extraFile.flush();
		testNames.clear();
		testResults.clear();
	}
	resFile.close();
	extraFile.close();

	return 0;
}