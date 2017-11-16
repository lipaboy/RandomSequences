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

void PseudoRandomSequences::runTests(std::vector<bool> const & epsilon,
	std::vector<std::string> & testNames,
	bool isSaveNames,
	std::vector<double> & testResults,
	std::string const & testKey,
	std::string const & inputFile) {
	const int EPSILON_SIZE = int(epsilon.size());
	// #Parameterized
	if (testKey[0] == '1') {
		// ! Each bit means 0 or 1 (you can't pass to bookStackTest 0 or 1 in whole byte for example)
		for (int dim = 2; dim <= 8; dim *= 2) {
			std::string sizeStr = std::to_string(EPSILON_SIZE);
			std::string dimStr = std::to_string(dim);
			std::string filename = (inputFile);
			std::vector<const char *> arguments{ "bs.exe",
				"-f", filename.c_str(),
				"-n", sizeStr.c_str(),	// file size (in bits)
				"-w", dimStr.c_str(),				// word size (or alphabet symbol length (see yourself book stack version)
													//"-b", "0",				// blank between words
													//"-u", "32"				// size of upper part book stack
			};
			if (isSaveNames) testNames.push_back("BookStackTest_" + dimStr);
			testResults.push_back(bookStackTestMain(int(arguments.size()), &arguments[0]));
		}
	}
	if (testKey[1] == '1') {
		if (isSaveNames) testNames.push_back("Frequency");
		testResults.push_back(Frequency(EPSILON_SIZE, epsilon));
	}
	// #Parameterized
	if (testKey[2] == '1') {
		//doesn't equal frequency monobit with M = 1
		if (isSaveNames) testNames.push_back("BlockFrequency_" + std::to_string(5));
		testResults.push_back(BlockFrequency(5, EPSILON_SIZE, epsilon));
	}
	if (testKey[3] == '1') {
		if (isSaveNames) testNames.push_back("Runs");
		testResults.push_back(Runs(EPSILON_SIZE, epsilon));
	}
	if (testKey[4] == '1') {
		if (isSaveNames) testNames.push_back("LongestRunOfOnes");
		testResults.push_back(LongestRunOfOnes(EPSILON_SIZE, epsilon));
	}
	if (testKey[5] == '1') {
		if (isSaveNames) testNames.push_back("Rank");
		testResults.push_back(Rank(EPSILON_SIZE, epsilon));
	}
	if (testKey[6] == '1') {
		// Has a little difference between results of my own discreteFourier Test version
		if (isSaveNames) testNames.push_back("DiscreteFourierTransform");
		testResults.push_back(DiscreteFourierTransform(EPSILON_SIZE, epsilon));
	}
	// #Parameterized
	if (testKey[7] == '1') {
		int blockSize = 5;
		// from 2 to 16
		if (isSaveNames) testNames.push_back("NonOverlappingTemplateMatchings_" + std::to_string(5));
		auto result = (blockSize <= 1) ? std::vector<double>()
			: NonOverlappingTemplateMatchings(blockSize, EPSILON_SIZE, epsilon);
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
		if (isSaveNames) testNames.push_back("OverlappingTemplateMatchings_" + std::to_string(5));
		testResults.push_back(OverlappingTemplateMatchings(5, EPSILON_SIZE, epsilon));
	}
	if (testKey[9] == '1') {
		if (isSaveNames) testNames.push_back("Universal");
		testResults.push_back(Universal(EPSILON_SIZE, epsilon));
	}
	// #Parameterized
	if (testKey[10] == '1') {		// think: neccessary try all the variant of blockSize (read documentation of test)
		int blockSize = 25;
		if (isSaveNames) testNames.push_back("LinearComplexity_" + std::to_string(blockSize));
		testResults.push_back((blockSize <= 3) ? -1. : LinearComplexity(blockSize, EPSILON_SIZE, epsilon));
	}
	// #Parameterized
	if (testKey[11] == '1') {// think: neccessary try all the variant of blockSize (read documentation of test)
		auto res = Serial(5, EPSILON_SIZE, epsilon);
		if (isSaveNames) testNames.push_back("Serial_" + std::to_string(5) + "_1");
		testResults.push_back(res.first);
		if (isSaveNames) testNames.push_back("Serial_" + std::to_string(5) + "_2");
		testResults.push_back(res.second);
	}
	// #Parameterized
	if (testKey[12] == '1') {// think: neccessary try all the variant of blockSize (read documentation of test)
		if (isSaveNames) testNames.push_back("ApproximateEntropy_" + std::to_string(5));
		// (M + 1) - bit block is used to compare
		testResults.push_back(ApproximateEntropy(5, EPSILON_SIZE, epsilon));
	}
	if (testKey[13] == '1') {
		auto res = CumulativeSums(EPSILON_SIZE, epsilon);
		if (isSaveNames) testNames.push_back("CumulativeSums_1");
		testResults.push_back(res.first);
		if (isSaveNames) testNames.push_back("CumulativeSums_1");
		testResults.push_back(res.second);
	}
	if (testKey[14] == '1') {
		if (isSaveNames) testNames.push_back("RandomExcursions");
		auto result = RandomExcursions(EPSILON_SIZE, epsilon);
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
		if (isSaveNames) testNames.push_back("RandomExcursionsVariant");
		auto result = RandomExcursionsVariant(EPSILON_SIZE, epsilon);
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
