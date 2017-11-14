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

int PseudoRandomSequences::beaconRun(int argc, char * argv[]) {
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
			<< "first file id, last file id)"
			<< endl;
		return -1;
	}

	// ---------------------------------------------------

	size_t firstId = boost::lexical_cast<size_t>(argv[2]);
	size_t lastId = boost::lexical_cast<size_t>(argv[3]);
	std::ofstream resFile;
	std::ofstream extraFile;
	resFile.open("res" + std::to_string(firstId) + "-" + std::to_string(lastId) + ".dat",
		std::ios::out | std::ios::trunc);
	extraFile.open("extra" + std::to_string(firstId) + "-" + std::to_string(lastId) + ".dat",
		std::ios::out | std::ios::trunc);

	std::string inputFilename = "input";
	Sequence result;

	for (size_t fileIndex = firstId; fileIndex <= lastId; fileIndex *= 2) {
		size_t inputSize = 1024u * fileIndex;

		//-------------Input----------------//
		{
			std::ifstream inFile;
			inFile.open(inputFilename + std::to_string(fileIndex) + ".dat", std::ios::in);
			inFile >> std::skipws;
			auto iter = std::istream_iterator<char>(inFile);
			for (size_t i = 0; i < inputSize
				&& iter != std::istream_iterator<char>()
				; i++, iter++) {
				// bool per char
				result.push_back((*iter) == '1');
			}
			inFile.close();
		}
		
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

		//----------------Tests-----------------//

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


void PseudoRandomSequences::runTests(std::vector<bool> const & epsilon, 
									std::vector<std::string> & testNames,
									std::vector<double> & testResults,
									std::string const & testKey,
									int fileIndex,
									std::string const & inputFile) {
	const int EPSILON_SIZE = epsilon.size();
	// #Parameterized
	if (testKey[0] == '1') {
		// ! Each bit means 0 or 1 (you can't pass to bookStackTest 0 or 1 in whole byte for example)
		for (int dim = 1; dim <= 8; dim++) {
			std::string sizeStr = std::to_string(EPSILON_SIZE);
			std::string dimStr = std::to_string(dim);
			std::string filename = (inputFile + std::to_string(fileIndex) + ".tmp");
			std::vector<const char *> arguments{ "bs.exe",
				"-f", filename.c_str(),
				"-n", sizeStr.c_str(),	// file size (in bits)
				"-w", dimStr.c_str(),				// word size (or alphabet symbol length (see yourself book stack version)
													//"-b", "0",				// blank between words
													//"-u", "32"				// size of upper part book stack
			};
			testNames.push_back("BookStackTest_" + dimStr);
			testResults.push_back(bookStackTestMain(int(arguments.size()), &arguments[0]));
		}
	}
	if (testKey[1] == '1') {
		testNames.push_back("Frequency");
		testResults.push_back(Frequency(EPSILON_SIZE));
	}
	// #Parameterized
	if (testKey[2] == '1') {
		//doesn't equal frequency monobit with M = 1
		testNames.push_back("BlockFrequency_" + std::to_string(5));
		testResults.push_back(BlockFrequency(5, EPSILON_SIZE));
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
	// #Parameterized
	if (testKey[7] == '1') {
		int blockSize = 5;
		// from 2 to 16
		testNames.push_back("NonOverlappingTemplateMatchings_" + std::to_string(5));
		auto result = (blockSize <= 1) ? std::vector<double>()
			: NonOverlappingTemplateMatchings(blockSize, EPSILON_SIZE);
		double average = 0.;
		for (auto elem : result) {
			average += (elem >= ALPHA);
		}
		size_t size = result.size();
		average /= size;
		testResults.push_back(size == 0 ? -1.
			: average + size * (ALPHA - (size - 1.) / size + 1e-3) * (1. - average));
	}
	// #Parameterized
	if (testKey[8] == '1') {
		testNames.push_back("OverlappingTemplateMatchings_" + std::to_string(5));
		testResults.push_back(OverlappingTemplateMatchings(5, EPSILON_SIZE));
	}
	if (testKey[9] == '1') {
		testNames.push_back("Universal");
		testResults.push_back(Universal(EPSILON_SIZE));
	}
	// #Parameterized
	if (testKey[10] == '1') {		// think: neccessary try all the variant of blockSize (read documentation of test)
		int blockSize = 25;
		testNames.push_back("LinearComplexity_" + std::to_string(blockSize));
		testResults.push_back((blockSize <= 3) ? -1. : LinearComplexity(blockSize, EPSILON_SIZE));
	}
	// #Parameterized
	if (testKey[11] == '1') {// think: neccessary try all the variant of blockSize (read documentation of test)
		auto res = Serial(5, EPSILON_SIZE);
		testNames.push_back("Serial_" + std::to_string(5) + "_1");
		testResults.push_back(res.first);
		testNames.push_back("Serial_" + std::to_string(5) + "_2");
		testResults.push_back(res.second);
	}
	// #Parameterized
	if (testKey[12] == '1') {// think: neccessary try all the variant of blockSize (read documentation of test)
		testNames.push_back("ApproximateEntropy_" + std::to_string(7));
		// (M + 1) - bit block is used to compare
		testResults.push_back(ApproximateEntropy(7, EPSILON_SIZE));
	}
	if (testKey[13] == '1') {
		auto res = CumulativeSums(EPSILON_SIZE);
		testNames.push_back("CumulativeSums_1");
		testResults.push_back(res.first);
		testNames.push_back("CumulativeSums_1");
		testResults.push_back(res.second);
	}
	if (testKey[14] == '1') {
		testNames.push_back("RandomExcursions");
		auto result = RandomExcursions(EPSILON_SIZE);
		double average = 0.;
		for (auto elem : result) {
			average += (elem >= ALPHA);
		}
		size_t size = result.size();
		average /= size;
		testResults.push_back(size == 0 ? -1.
			: average + size * (ALPHA - (size - 1.) / size + 1e-3) * (1. - average));
	}
	if (testKey[15] == '1') {		// For more longer sequences (> 1e6)
		testNames.push_back("RandomExcursionsVariant");
		auto result = RandomExcursionsVariant(EPSILON_SIZE);
		double average = 0.;
		for (auto elem : result) {
			average += (elem >= ALPHA);
		}
		size_t size = result.size();
		average /= size;
		testResults.push_back(size == 0 ? -1.
			: average + size * (ALPHA - (size - 1.) / size + 1e-3) * (1. - average));
	}
}
