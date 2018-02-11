#include "pseudoRandomSequences.h"

#include <iterator>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>
#include <exception>
#include <cstdio>

#include "statTests/include/stat_fncs.h"
#include "lipaboyLibrary/src/maths/fixed_precision_number.h"

using namespace PseudoRandomSequences;


using std::vector;
using std::pair;
using std::string;
using std::cout;
using std::endl;

    // TODO: take off the responsibility of return names of active tests and give it to another function with argument testKey

void PseudoRandomSequences::runTests(
	BoolIterator epsilonBegin,
    BoolIterator epsilonEnd,
	std::vector<double> & testResults,
    std::string const & testKey
        )
{
	const int EPSILON_SIZE = int(std::distance(epsilonBegin, epsilonEnd));
    TestParameters testParameters(EPSILON_SIZE);

	// #Parameterized
	if (testKey[0] == '1') {
        string inputFile = "bookStackInput.dat";
        {
            std::ofstream outFile;
            outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
            outFile.open(inputFile, std::ios::trunc);
            auto outIter = std::ostream_iterator<char>(outFile);
            int bitPos = 0;
            char buffer = 0;
            for (auto iter = epsilonBegin; iter != epsilonEnd; iter++) {
                buffer |= (*iter) << (bitPos++);
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

        auto start = my_get_current_clock_time();
        // ! Each bit means 0 or 1 (you can't pass to bookStackTest 0 or 1 in whole byte for example)
        for (auto param : testParameters.bookStackTest) {
            string sizeStr = std::to_string(EPSILON_SIZE);
            string dimStr = std::to_string(param.dimension);
            string upperPartStr = std::to_string(param.upperPart);
            string filename = inputFile;
            std::vector<const char *> arguments{ "bs.exe",
                "-f", filename.c_str(),
                "-n", sizeStr.c_str(),	// file size (in bits)
                "-w", dimStr.c_str(),				// word size (or alphabet symbol length (see yourself book stack version)
                //"-b", "0",				// blank between words
                "-u", upperPartStr.c_str()				// size of upper part book stack
            };
            if (param.upperPart > (1LL << 28))
                continue;
            //std::cout << "up=" << upperPartStr << ", dim = " << dimStr << std::endl;

            testResults.push_back(bookStackTestMain(int(arguments.size()), &arguments[0]));
        }
        std::remove(inputFile.c_str());
        cout << "Book stack test time: "
             << getTimeDifferenceInMillis(start, my_get_current_clock_time()) << endl;
	}
	if (testKey[1] == '1') {
        auto start = my_get_current_clock_time();
		testResults.push_back(Frequency(EPSILON_SIZE, epsilonBegin));
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	// #Parameterized
    if (testKey[2] == '1') {
        auto start = my_get_current_clock_time();
        for (auto blockSize : testParameters.blockFrequencyTest) {
            //doesn't equal frequency monobit with M = 1
            testResults.push_back(BlockFrequency(blockSize, EPSILON_SIZE, epsilonBegin));
        }
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	if (testKey[3] == '1') {
        auto start = my_get_current_clock_time();
		testResults.push_back(Runs(EPSILON_SIZE, epsilonBegin));
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	if (testKey[4] == '1') {
        auto start = my_get_current_clock_time();
		testResults.push_back(LongestRunOfOnes(EPSILON_SIZE, epsilonBegin));
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	if (testKey[5] == '1') {
        auto start = my_get_current_clock_time();
		testResults.push_back(Rank(EPSILON_SIZE, epsilonBegin));
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	if (testKey[6] == '1') {
        auto start = my_get_current_clock_time();
		// Has a little difference between results of my own discreteFourier Test version
		testResults.push_back(DiscreteFourierTransform(EPSILON_SIZE, epsilonBegin));
        ////cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	// #Slow test
	// #Parameterized
    if (testKey[7] == '1') {	// TODO: need to check
        auto start = my_get_current_clock_time();
        //2 - is minimum (depends on existing files)
		// from 2 to 16
        for (auto param : testParameters.nonOverlappingTemplateMatchingsTest) {
			std::vector<double> temp;
			temp = NonOverlappingTemplateMatchings(param, EPSILON_SIZE, epsilonBegin);
			double average = 0.;
			for (auto elem : temp) {
				average += (elem >= ALPHA);
			}
			const size_t size = temp.size();
			average /= size;
			// TODO: here
			testResults.push_back(size == 0 ? -1.
				: average + size * (ALPHA - (size - 1.) / size + 1e-3) * (1. - average));
		}
        ////cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	// #Parameterized
	if (testKey[8] == '1') {
        auto start = my_get_current_clock_time();

        for (auto param : testParameters.overlappingTemplateMatchingsTest) {
			testResults.push_back(OverlappingTemplateMatchings(param, EPSILON_SIZE, epsilonBegin));
		}
        ////cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	if (testKey[9] == '1') {
        auto start = my_get_current_clock_time();
        testResults.push_back(
            Universal(EPSILON_SIZE, epsilonBegin)
        );
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
    // #Parameterized
    if (testKey[10] == '1') {		// think: neccessary try all the variant of blockSize (read documentation of test)
        for (auto param : testParameters.linearComplexityTest) {
            testResults.push_back(
                LinearComplexity(param, EPSILON_SIZE, epsilonBegin)
            );
        }
    }
	// #TheSlowest
	// #Parameterized
	if (testKey[11] == '1') {// think: neccessary try all the variant of blockSize (read documentation of test)
        auto start = my_get_current_clock_time();
        for (auto param : testParameters.serialTest) {
            auto res = Serial(param, EPSILON_SIZE, epsilonBegin);
            testResults.push_back(res.first);
			testResults.push_back(res.second);
		}
        ////cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
	// #Slow
	// #Parameterized
	if (testKey[12] == '1') {// think: neccessary try all the variant of blockSize (read documentation of test)
        auto start = my_get_current_clock_time();
        for (auto param : testParameters.approximateEntropyTest) {
			// (M + 1) - bit block is used to compare
			testResults.push_back(ApproximateEntropy(param, EPSILON_SIZE, epsilonBegin));
		}
        ////cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
    if (testKey[13] == '1') {
        auto start = my_get_current_clock_time();
		auto res = CumulativeSums(EPSILON_SIZE, epsilonBegin);
		testResults.push_back(res.first);
		testResults.push_back(res.second);
        //cout << "Time: " << my_get_current_clock_time() - start << endl;
	}
    if (testKey[14] == '1') {
        auto result =
              RandomExcursions(EPSILON_SIZE, epsilonBegin);
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
        auto result =
              RandomExcursionsVariant(EPSILON_SIZE, epsilonBegin);
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

vector<string> PseudoRandomSequences::getStatisticTestNames(string testKey, size_t sequenceSize) {
    vector<string> testNames;
    TestParameters testParameters(sequenceSize);
std::cout << sequenceSize << std::endl;
    if (testKey[0] == '1')
        for (auto param : testParameters.bookStackTest)
            testNames.push_back("BookStackTest_" + std::to_string(param.dimension)
                                + "dim_" + std::to_string(param.upperPart) + "up");
    if (testKey[1] == '1')
        testNames.push_back("Frequency");
    if (testKey[2] == '1')
        for (auto blockSize : testParameters.blockFrequencyTest)
            testNames.push_back("BlockFrequency_" + std::to_string(blockSize));
    if (testKey[3] == '1')
        testNames.push_back("Runs");;
    if (testKey[4] == '1')
        testNames.push_back("LongestRunOfOnes");
    if (testKey[5] == '1')
        testNames.push_back("Rank");
    if (testKey[6] == '1')
        testNames.push_back("DiscreteFourierTransform");
    if (testKey[7] == '1')
        for (auto param : testParameters.nonOverlappingTemplateMatchingsTest)
            testNames.push_back("NonOverlappingTemplateMatchings_"
                + std::to_string(param));
    if (testKey[8] == '1')
        for (auto param : testParameters.overlappingTemplateMatchingsTest)
            testNames.push_back("OverlappingTemplateMatchings_" + std::to_string(param));
    if (testKey[9] == '1')
        testNames.push_back("Universal");
    if (testKey[10] == '1')
        for (auto param : testParameters.linearComplexityTest)
            testNames.push_back("LinearComplexity_" + std::to_string(param));
    if (testKey[11] == '1')
        for (auto param : testParameters.serialTest) {
            testNames.push_back("Serial_" + std::to_string(param) + "_1");
            testNames.push_back("Serial_" + std::to_string(param) + "_2");
        }
    if (testKey[12] == '1')
        for (auto param : testParameters.approximateEntropyTest)
            testNames.push_back("ApproximateEntropy_" + std::to_string(param));
    if (testKey[13] == '1') {
        testNames.push_back("CumulativeSums_1");
        testNames.push_back("CumulativeSums_2");
    }
    if (testKey[14] == '1')
        testNames.push_back("RandomExcursions");
    if (testKey[15] == '1')
        testNames.push_back("RandomExcursionsVariant");

    return std::move(testNames);
}

PseudoRandomSequences::TestParameters::TestParameters(uint64_t EPSILON_SIZE)
    : blockFrequencyTest({ 2, EPSILON_SIZE / 4, EPSILON_SIZE / 2 }),
      nonOverlappingTemplateMatchingsTest({ 3 }),
      overlappingTemplateMatchingsTest({ 2, EPSILON_SIZE / 2, EPSILON_SIZE })
{
    for (int upperPart = 0; upperPart < 3; upperPart++) {
        for (uint64_t dim = 8; dim <= 32; dim *= 2) {	//8, 16, 32
            uint64_t upperPartSize = (upperPart == 0) ? 16LL
                : ((upperPart == 1) ? (1LL << (dim / 2)) : (1LL << (dim - 1)));
            if (upperPartSize > (1LL << 28))
                continue;
            bookStackTest.push_back({ upperPartSize, dim });
        }
    }

    auto sqrtSize = uint64_t(std::floor(std::pow(EPSILON_SIZE, 0.5)));
    auto sqrtSqrtSize = uint64_t(std::floor(std::pow(EPSILON_SIZE, 0.25)));
    linearComplexityTest = { //8, sqrtSize / 2, sqrtSize
                            8, sqrtSqrtSize,
        //            sqrtSize
    };
    int logSize = int(std::floor(std::log2(EPSILON_SIZE)) - 2);
    serialTest = { 3 };
    approximateEntropyTest = { 1 };
}


TimeType PseudoRandomSequences::my_get_current_clock_time() {
#ifdef __linux__
//       struct tms curr;
//       long clocks_per_sec = sysconf(_SC_CLK_TCK);
//       times(&curr);
//       return (int)std::round(curr.tms_utime / (double)clocks_per_sec * 1000.);
   return std::chrono::steady_clock::now();
#elif _WIN32    //Windows
   return clock();
#endif
}

using std::chrono::time_point;

int PseudoRandomSequences::getTimeDifferenceInMillis(TimeType const & from, TimeType const & to) {
#ifdef __linux__
   return std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count();
#elif _WIN32   //Windows
   return to - from;
#endif
}

