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
	if (argc < 4 || (len = std::strlen(argv[1])) < TEST_COUNT) {
		cout << "Not enough parameters ( testKey ("
			<< TEST_COUNT << ", current = " << len << "), "
			<< "input possibility, first size, last size)"
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	std::string inputFilename = "input";
	// Kbits
	int inputOppositePossibility = static_cast<int>(
		std::round(1.0 / boost::lexical_cast<double>(argv[2]))
		);
	size_t firstSize = boost::lexical_cast<size_t>(argv[3]);
	size_t lastSize = boost::lexical_cast<size_t>(argv[4]);
	std::vector<string> generatorNames{ 
		"minstd_rand", 
		//"knuth_b", 
		"ranlux48", 
		//"random_device" 
	};
	std::minstd_rand generatorMinstdRand;
	std::knuth_b generatorKnuthB;
	std::ranlux48 generatorRanlux48;	//failure with normal_distribution and with chi_squared_distribution
	std::random_device generatorRandomDevice;

	for (int iGen = 0; iGen < generatorNames.size(); iGen++) {
		std::string & genName = generatorNames[iGen];
		
		std::ofstream resFile;
	//	std::ofstream extraFile;
		resFile.open("resStdGenerators_" + genName + "_" + std::to_string(firstSize) 
			+ "-" + std::to_string(lastSize) + ".dat",
			std::ios::out | std::ios::trunc);
		//extraFile.open("extraStdGenerators_" + genName + ".dat",
		//	std::ios::out | std::ios::trunc);

		for (size_t iSize = firstSize; iSize <= lastSize; iSize *= 4) {
			size_t inputSize = 1024u * iSize;
			
			cout << genName << endl;
			cout << "Sequence size = " << inputSize << endl;
			//-------------Input----------------//

			//Sequence seq(inputSize);
			std::vector<bool> epsilon(inputSize);

			vector<double> testResults;
			vector<string> testNames = { "" };
			vector<double> currResults;
			std::string outFilename = "out.tmp";
			string testKey(argv[1]);

			currResults.reserve(60u);
			int traversalCount = (epsilon.size() < static_cast<int>(1e5)) ? 10 : 10;
			for (int jTraver = 0; jTraver < traversalCount; jTraver++) 
			{
				// TODO: bad computation of input possibility (use another input format) (bad because 2/3)
				std::normal_distribution<double> distribution(4.5, 2.0);		//doesn't failure with random_device generator
				//std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0)

				if ("minstd_rand" == genName) {
					std::generate_n(epsilon.begin(), inputSize,
						[&inputOppositePossibility, &generatorMinstdRand, &distribution, &inputSize]() -> bool {
						return (int(std::round(distribution(generatorMinstdRand))) % inputOppositePossibility == 0);
					});
				}
				else if ("knuth_b" == genName) {
					std::generate_n(epsilon.begin(), inputSize,
						[&inputOppositePossibility, &generatorKnuthB, &distribution, &inputSize]() -> bool {
						return (int(std::round(distribution(generatorKnuthB))) % inputOppositePossibility == 0);
					});
				}
				else if ("ranlux48" == genName) {
					std::generate_n(epsilon.begin(), inputSize,
						[&inputOppositePossibility, &generatorRanlux48, &distribution, &inputSize]() -> bool {
						return (int(std::round(distribution(generatorRanlux48))) % inputOppositePossibility == 0);
					});
				}
				else if ("random_device" == genName) {
					std::generate_n(epsilon.begin(), inputSize,
						[&inputOppositePossibility, &generatorRandomDevice, &distribution, &inputSize]() -> bool {
						return (int(std::round(distribution(generatorRandomDevice))) % inputOppositePossibility == 0);
					});
				}
				else {
					continue;
				}

				std::copy(epsilon.begin(), epsilon.begin() + 3, std::ostream_iterator<bool>(cout, ""));
				std::cout << std::endl;

				//-------------Output----------------//
			
				{
					std::ofstream outFile;
					outFile.open(outFilename, std::ios::out | std::ios::trunc);
					//std::copy(result.begin(), result.end(), std::ostream_iterator<bool>(outFile, ""));
					auto outIter = std::ostream_iterator<char>(outFile);
					int bitPos = 0;
					char buffer = 0;
					for (auto val : epsilon) {
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

				//----------------Tests-----------------//
				{
					runTests(epsilon, testNames, (iSize <= firstSize) && (jTraver <= 0), 
						currResults, testKey, outFilename);
					if (jTraver == 0)
						testResults.assign(currResults.size(), 0);
					std::transform(currResults.begin(), currResults.end(), testResults.begin(),
						testResults.begin(),
						[](double p_value, double count) -> double { return (std::abs(p_value - -1.) < 1e-5)
											? count : (p_value < ALPHA) + count; }
					);
					currResults.clear();
				}
			}

			//----------------Write results-----------------//
			{
				if (iSize <= firstSize) {
					std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
					resFile << endl;
				}
				resFile << iSize << "_Kbits\t";
				std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"));
				resFile << endl;
			}
			//----------------Extra infos-----------------//

			resFile.flush();
			testNames.clear();
			//extraFile.flush();
		}
		resFile.close();
		//extraFile.close();
	}
	

	return 0;
}