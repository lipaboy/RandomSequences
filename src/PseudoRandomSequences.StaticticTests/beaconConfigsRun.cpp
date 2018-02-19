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

#include "statTests/include/stat_fncs.h"
#include "lipaboyLibrary/src/maths/fixed_precision_number.h"

using namespace PseudoRandomSequences;
using namespace std::chrono;

//TODO: try to use GoogleTests

//typedef std::vector<char> Sequence;

const int TEST_COUNT = 16;

int PseudoRandomSequences::beaconRun(int argc, char * argv[]) {
	time_t t;
    std::srand((unsigned int)(std::time(&t)));
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

    std::string inputFilename = "beacon/input";
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

        //----------------Tests-----------------//
        string testKey(argv[1]);
        vector<string> testNames = getStatisticTestNames(testKey, result.size());
        vector<double> testResults;
        {
            Sequence epsilon;
            epsilon = std::move(result);

            runTests(epsilon.begin(), epsilon.end(), testResults, testKey, "beacon"
                //outFilename + std::to_string(fileIndex) + ".tmp"
                     );
        }
		//----------------Write results-----------------//
		{
			if (fileIndex <= firstId) {
                resFile << "\t";
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
                extraFile << "\t";
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


