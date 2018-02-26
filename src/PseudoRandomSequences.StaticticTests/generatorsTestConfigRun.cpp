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

//typedef std::vector<char> Sequence;

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

    //-----------------------------Input data-----------------------------//

    int sizesStartIndex = 5;
    size_t len = (argc > 1) ? std::strlen(argv[1]) : 0;
    size_t countSizes = (argc >= sizesStartIndex + 1) ? std::atoi(argv[sizesStartIndex]) : 0;
    if (countSizes <= 0 || argc < sizesStartIndex + 1 + countSizes || len < TEST_COUNT) {
		cout << "Not enough parameters ( testKey ("
			<< TEST_COUNT << ", current = " << len << "), "
            << "input possibility, "
            << "traversal count, generator name, count of seq. sizes (N = "
            << countSizes << "), size 1, size 2, .., size N)"
			<< endl;
		return -1;
	}
    int inputOppositePossibility = static_cast<int>(
        std::round(1.0 / boost::lexical_cast<double>(argv[2]))
    );
    std::vector<string> generatorNames { argv[4] };
    const int TRAVERSAL_COUNT_SMALL = std::atoi(argv[3]);
    string testKey(argv[1]);
    vector<size_t> seqSizes;
    for (int i = sizesStartIndex + 1; i < argc; i++)
        seqSizes.push_back(std::atoi(argv[i]));


	// TODO: add test performance for conversation and tests

	// TODO: bad computation of input possibility (use another input format) (bad because 2/3)
    // Kbits

	std::minstd_rand generatorMinstdRand;
    std::minstd_rand0 generatorMinstdRand0;
	std::knuth_b generatorKnuthB;
    std::ranlux24 generatorRanlux24;
	std::ranlux48 generatorRanlux48;	//failure with normal_distribution and with chi_squared_distribution
	std::random_device generatorRandomDevice;
    std::default_random_engine generatorDefaultRandomEngine;
    std::mt19937_64 generatorMt19937_64;

    auto wholeTimeExpend = my_get_current_clock_time();
	for (int iGen = 0; iGen < generatorNames.size(); iGen++) {
		std::string & genName = generatorNames[iGen];
		
		std::ofstream resFile;
//        std::ofstream extraFile;
        string outFilename = "resStdGenerators_" + genName;
        for (auto iSize : seqSizes)
            outFilename += "_" + std::to_string(iSize);
        resFile.open(outFilename + ".dat", std::ios::out | std::ios::trunc);
//        extraFile.open("extraStdGenerators_" + genName + "_" + std::to_string(firstSize)
//                       + "-" + std::to_string(lastSize) + ".dat",
//            std::ios::out | std::ios::trunc);

		//--------------------Container---------------------//

		uint32_t atom = 1u;//1000u;

        const int TRAVERSAL_COUNT_LARGE = TRAVERSAL_COUNT_SMALL;
        const size_t TRAVERSAL_THRESHOLD = size_t(1e5);
        Sequence epsilon;
		tp.n = 0;
		tp.numOfBitStreams = 1;
        for (auto jSize : seqSizes) {
			tp.n += jSize * atom * ((jSize < TRAVERSAL_THRESHOLD) ? TRAVERSAL_COUNT_LARGE
				: TRAVERSAL_COUNT_SMALL);
		}

		// TODO: Too much memory allocations
        std::normal_distribution<double> distribution(4.5, 2.0);		//doesn't failure with random_device generator
        //std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0)
        if ("minstd_rand" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorMinstdRand, &distribution]() -> bool {
                return (int(std::round(distribution(generatorMinstdRand))) % inputOppositePossibility == 0);
            });
        }
        else if ("minstd_rand0" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorMinstdRand0, &distribution]() -> bool {
                return (int(std::round(distribution(generatorMinstdRand0))) % inputOppositePossibility == 0);
            });
        }
        else if ("knuth_b" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorKnuthB, &distribution]() -> bool {
                return (int(std::round(distribution(generatorKnuthB))) % inputOppositePossibility == 0);
            });
        }
        else if ("ranlux24" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorRanlux24, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRanlux24))) % inputOppositePossibility == 0);
            });
        }
        else if ("ranlux48" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorRanlux48, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRanlux48))) % inputOppositePossibility == 0);
            });
        }
        else if ("random_device" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorRandomDevice, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRandomDevice))) % inputOppositePossibility == 0);
            });
        }
        else if ("rand" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorDefaultRandomEngine, &distribution]() -> bool {
                return ((std::rand() % 2) == 0);
            });
        }
        else if ("mt19937_64" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorMt19937_64, &distribution]() -> bool {
                return (int(std::round(distribution(generatorMt19937_64))) % inputOppositePossibility == 0);
            });
        }
        else if ("bad" == genName) {
            std::generate_n(std::back_inserter(epsilon), tp.n,
                [&inputOppositePossibility, &generatorMt19937_64, &distribution]() -> bool {
                static int i = 0;
                return (i++ < tp.n);
            });
        }
        else if ("lcg" == genName)
			epsilon = lcg();
//		else if ("SHA1" == genName) //wrong given results
//			epsilon = SHA1();
        else if ("modExp" == genName)       //slow
			epsilon = modExp();
        else if ("bbs" == genName)          //slow
			epsilon = bbs();
		else if ("exclusiveOR" == genName)
			epsilon = exclusiveOR();
        else if ("micali_schnorr" == genName)
            epsilon = micali_schnorr();
        else if ("cubicRes" == genName)
            epsilon = cubicRes();
        else if ("quadRes1" == genName)
            epsilon = quadRes1();
        else if ("quadRes2" == genName)
            epsilon = quadRes2();
        else {
            cout << "Error: no such generator" << endl;
            return -1;
        }

//        std::ifstream inData;
//        //linux
//        inData.open("data/data.fourierExample", std::ios::in);
//        //windows
//        //inData.open("data\\data.fourierExample", std::ios::in);
//        inData.seekg(0);
//        for (auto elem : epsilon) {
//            if (inData.eof())
//                break;
//            char ch;
//            inData >> ch;
//            elem = (ch == '1') ? true : false;
//                    //false;
//           // cout << elem;
//        }
//        cout << endl;
//        inData.close();

		size_t accumulatorSize = 0u;
        for (auto iSize : seqSizes) {
			size_t inputSize = //1024u 
				atom * iSize;
			
			cout << ">-----------------------New Size-----------------------<" << endl;
			cout << endl << genName << endl;
			cout << "Sequence size = " << inputSize << endl;
            //-------------Input----------------//

            vector<string> testNames =
                    getStatisticTestNames(testKey, inputSize);
            vector<double> testResults(testNames.size());
            vector<double> currResults;

            //currResults.reserve(60u);     // better don't do it because you can miss segmentation fault
            int traversalCount = TRAVERSAL_COUNT_LARGE;
			cout << seqSizes.size() << " " << traversalCount << " " << generatorNames.size() << endl;

#pragma omp parallel for private(currResults) shared(genName, traversalCount, testKey, testResults)
			for (int jTraver = 0; jTraver < traversalCount; jTraver++) 
			{
				// Generator factory
                MyBoolRange epsilonRange(epsilon.begin(), epsilon.end());
                {
                    {
                        auto iterBegin = epsilon.begin();
                        auto iterEnd = epsilon.begin();
                        #pragma omp critical
                        {
                            std::advance(iterBegin, accumulatorSize);
                            std::advance(iterEnd, accumulatorSize + inputSize);
                            accumulatorSize += inputSize;
                        }
                        epsilonRange = MyBoolRange(iterBegin, iterEnd);
					}
				}

//				{
//					auto iterEnd = epsilonRange.begin();
//					std::advance(iterEnd, 10);
//					std::copy(epsilonRange.begin(), iterEnd,
//						std::ostream_iterator<bool>(cout, ""));
//					std::cout << std::endl;
//                }

				//----------------Tests-----------------//
				{
                    runTests(epsilonRange.begin(), epsilonRange.end(), currResults, testKey, genName);
                    #pragma omp critical
                    {
                        std::transform(currResults.begin(), currResults.end(), //first source
                            testResults.begin(),                                //second source
                            testResults.begin(),                                //destination
                            [](double p_value, double count) -> double {
                                return ((p_value < 0.) ? (count - 1000.) : ((p_value < 0.05) + count)); }
                        // p_value < ALPHA - it is failure
                        );
                        for (auto pValue : currResults) {
                            cout << pValue << " ";
                        }
                        cout << endl;
                    }
                    currResults.clear();
                }
			}

			//----------------Write results-----------------//
			{
                if (iSize == seqSizes[0]) {
                    resFile << "\t";
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
					resFile << endl;
                }
                resFile << iSize << "_Kbits\t";
                std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"));
				resFile << endl;
			}
			//----------------Extra infos-----------------//
//            {
//                //if (iSize <= firstSize) {
//                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(extraFile, "\t"));
//                    extraFile << endl;
//                //}
//                extraFile << iSize << "_Kbits\t";
//                std::copy(currResults.begin(), currResults.end(), std::ostream_iterator<double>(extraFile, "\t"));
//                extraFile << endl;
//            }

            resFile.flush();
//            extraFile.flush();
			testNames.clear();
		}
		resFile.close();
//        extraFile.close();
	}

    cout << endl << "Whole time expend: "
         << getTimeDifferenceInMillis(wholeTimeExpend, my_get_current_clock_time()) / 1000
         << "sec." << endl;
	

	return 0;
}
