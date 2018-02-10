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
#include "statTests/include/generators.h"

using namespace PseudoRandomSequences;
using namespace std::chrono;

//TODO: try to use GoogleTests

typedef std::vector<bool> Sequence;

const int TEST_COUNT = 16;

namespace {
	class MyBoolRange {
	private:
		BoolIterator _begin, _end;
	public:
		MyBoolRange(BoolIterator beg, BoolIterator end) : _begin(beg), _end(end) {}
		BoolIterator const & begin() { return _begin; }
		BoolIterator const & end() { return _end; }
	};
}

int PseudoRandomSequences::generatorsTestConfigRun(int argc, char * argv[]) {
	time_t t;
    std::srand((unsigned int)(std::time(&t)));
	using std::string;
	using std::vector;
	using std::cout;
	using std::endl;


	size_t len = 0;
    if (argc < 5 || (len = std::strlen(argv[1])) < TEST_COUNT) {
		cout << "Not enough parameters ( testKey ("
			<< TEST_COUNT << ", current = " << len << "), "
            << "input possibility, first size, last size, "
            << "traversal count)"
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	std::string inputFilename = "input";
	// TODO: bad computation of input possibility (use another input format) (bad because 2/3)
	// Kbits
	int inputOppositePossibility = static_cast<int>(
		std::round(1.0 / boost::lexical_cast<double>(argv[2]))
		);
	size_t firstSize = boost::lexical_cast<size_t>(argv[3]);
	size_t lastSize = boost::lexical_cast<size_t>(argv[4]);
	std::vector<string> generatorNames{ 
		//"minstd_rand", 
			//"knuth_b", 
		//"ranlux48", 
			//"random_device",

        "lcg",    //fast
        //"SHA1",   //fast
       // "exclusiveOR"   //fast
       // "micali_schnorr"    //fast

        //"modExp", //slow
        //"bbs",    //slow
	};
	std::minstd_rand generatorMinstdRand;
	std::knuth_b generatorKnuthB;
	std::ranlux48 generatorRanlux48;	//failure with normal_distribution and with chi_squared_distribution
	std::random_device generatorRandomDevice;

    auto wholeTimeExpend = my_get_current_clock_time();
	for (int iGen = 0; iGen < generatorNames.size(); iGen++) {
		std::string & genName = generatorNames[iGen];
		
		std::ofstream resFile;
        std::ofstream extraFile;
		resFile.open("resStdGenerators_" + genName + "_" + std::to_string(firstSize) 
			+ "-" + std::to_string(lastSize) + ".dat",
			std::ios::out | std::ios::trunc);
        extraFile.open("extraStdGenerators_" + genName + "_" + std::to_string(firstSize)
                       + "-" + std::to_string(lastSize) + ".dat",
            std::ios::out | std::ios::trunc);

		//--------------------Container---------------------//

        const int TRAVERSAL_COUNT_SMALL = std::atoi(argv[5]);
        const int TRAVERSAL_COUNT_LARGE = TRAVERSAL_COUNT_SMALL;
		const size_t TRAVERSAL_THRESHOLD = size_t(1e5);
		size_t stepIterSize = 4;		// the step of size iteration (traversal step)
		std::vector<bool> epsilon;
		tp.n = 0;
		tp.numOfBitStreams = 1;
		for (size_t jSize = firstSize; jSize <= lastSize; jSize *= stepIterSize) {
			tp.n += jSize * 1024u * ((jSize < TRAVERSAL_THRESHOLD) ? TRAVERSAL_COUNT_LARGE
				: TRAVERSAL_COUNT_SMALL);
		}

		bool isStdGenerators = false;
		// TODO: Too much memory allocations
		if ("lcg" == genName)
			epsilon = lcg();
		else if ("SHA1" == genName)
			epsilon = SHA1();
		else if ("modExp" == genName)
			epsilon = modExp();
		else if ("bbs" == genName)
			epsilon = bbs();
		else if ("exclusiveOR" == genName)
			epsilon = exclusiveOR();
        else if ("micali_schnorr" == genName)
            epsilon = micali_schnorr();
		else
			isStdGenerators = true;

		MyBoolRange epsilonRange = { epsilon.begin(), epsilon.end() };

		size_t accumulatorSize = 0u;
		for (size_t iSize = firstSize; iSize <= lastSize; iSize *= stepIterSize) {
			size_t inputSize = 1024u * iSize;
			
			cout << ">-----------------------New Size-----------------------<" << endl;
			cout << endl << genName << endl;
			cout << "Sequence size = " << inputSize << endl;
			//-------------Input----------------//

			if (isStdGenerators)
				epsilon.resize(inputSize);

			vector<double> testResults;
			vector<string> testNames = { "" };
			vector<double> currResults;
			std::string outFilename = "out.tmp";
			string testKey(argv[1]);

			currResults.reserve(60u);
            int traversalCount =
                    (std::distance(epsilonRange.begin(), epsilonRange.end()) < TRAVERSAL_THRESHOLD)
                        ? TRAVERSAL_COUNT_LARGE : TRAVERSAL_COUNT_SMALL;

//#pragma omp parallel for reduction(+:accumulatorSize) private(epsilonRange) shared(genName, traversalCount)
			for (int jTraver = 0; jTraver < traversalCount; jTraver++) 
			{
				// Generator factory
				{
					std::normal_distribution<double> distribution(4.5, 2.0);		//doesn't failure with random_device generator
					//std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0)
					if ("minstd_rand" == genName) {
						std::generate(epsilonRange.begin(), epsilonRange.end(),
							[&inputOppositePossibility, &generatorMinstdRand, &distribution, &inputSize]() -> bool {
							return (int(std::round(distribution(generatorMinstdRand))) % inputOppositePossibility == 0);
						});
					}
					else if ("knuth_b" == genName) {
						std::generate(epsilonRange.begin(), epsilonRange.end(),
							[&inputOppositePossibility, &generatorKnuthB, &distribution, &inputSize]() -> bool {
							return (int(std::round(distribution(generatorKnuthB))) % inputOppositePossibility == 0);
						});
					}
					else if ("ranlux48" == genName) {
						std::generate(epsilonRange.begin(), epsilonRange.end(),
							[&inputOppositePossibility, &generatorRanlux48, &distribution, &inputSize]() -> bool {
							return (int(std::round(distribution(generatorRanlux48))) % inputOppositePossibility == 0);
						});
					}
					else if ("random_device" == genName) {
						std::generate(epsilonRange.begin(), epsilonRange.end(),
							[&inputOppositePossibility, &generatorRandomDevice, &distribution, &inputSize]() -> bool {
							return (int(std::round(distribution(generatorRandomDevice))) % inputOppositePossibility == 0);
						});
					}
					else {
						auto iterBegin = epsilon.begin();
						auto iterEnd = epsilon.begin();
						std::advance(iterBegin, accumulatorSize);
						std::advance(iterEnd, accumulatorSize + inputSize);
						epsilonRange = MyBoolRange(iterBegin, iterEnd);
						accumulatorSize += inputSize;
					}
				}

				{
					auto iterEnd = epsilonRange.begin();
					std::advance(iterEnd, 10);
					std::copy(epsilonRange.begin(), iterEnd,
						std::ostream_iterator<bool>(cout, ""));
					std::cout << std::endl;
                }

				//----------------Tests-----------------//
				{
					runTests(epsilonRange.begin(), epsilonRange.end(), testNames, 
						(iSize <= firstSize) && (jTraver <= 0), currResults, testKey);
					if (jTraver == 0)
						testResults.assign(currResults.size(), 0);
                    std::transform(currResults.begin(), currResults.end(), //first source
                        testResults.begin(),                                //second source
                        testResults.begin(),                                //destination
						[](double p_value, double count) -> double { return (std::abs(p_value - -1.) < 1e-5)
											? count : (p_value < ALPHA) + count; }
                    );
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
            {
                if (iSize <= firstSize) {
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(extraFile, "\t"));
                    extraFile << endl;
                }
                extraFile << iSize << "_Kbits\t";
                std::copy(currResults.begin(), currResults.end(), std::ostream_iterator<double>(extraFile, "\t"));
                extraFile << endl;
            }

            resFile.flush();
            extraFile.flush();
            currResults.clear();
			testNames.clear();
		}
		resFile.close();
        extraFile.close();
	}

    cout << endl << "Whole time expend: " << (my_get_current_clock_time() - wholeTimeExpend) / 1000.
         << "sec." << endl;
	

	return 0;
}
