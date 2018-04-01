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

#include <memory>

#include <openssl/sha.h>

#include "lipaboyLibrary/src/maths/fixed_precision_number.h"
#include "statTests/include/generators.h"

namespace statistical_tests_space {

using namespace std::chrono;

using std::string;
using std::vector;
using std::cout;
using std::endl;

//TODO: try to use GoogleTests

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

static Sequence readSequenceByBitFromFile(string const & inputFile, size_t sequenceSize) {
    // TODO: write catching exceptions

    using std::ifstream;
    typedef u_char BlockReadType;

    ifstream inFile;
    Sequence epsilon(sequenceSize);

    inFile.open(inputFile, std::ios::in);

    BlockReadType buffer;
    for (size_t i = 0; i < epsilon.size() / 8; i++) {
        char symbol;
        inFile >> symbol;
        buffer = static_cast<BlockReadType>(symbol);
        for (int bit = 0; bit < static_cast<int>(8 * sizeof(BlockReadType)); bit++) {
            epsilon[i * 8 + bit] = static_cast<BitSequence>((buffer & (1 << bit)) >> bit);
        }
    }
    std::copy_n(epsilon.begin(), 16, std::ostream_iterator<BitSequence>(cout));
    cout << endl;

    inFile.close();

    return std::move(epsilon);
}

static Sequence readSequenceByByteFromFile(string const & inputFile, size_t sequenceSize,
                                           char isZero = '0', bool isSpecialFormat = true) {
    // TODO: write catching exceptions

    using std::ifstream;

    ifstream inFile;
    Sequence epsilon(sequenceSize);

    inFile.open(inputFile, std::ios::in);

    for (int i = 0; i < static_cast<int>(epsilon.size()); i++) {
        char symbol;
        inFile >> symbol;
        epsilon[i] = (!isSpecialFormat && symbol == isZero)       //for others
                    ||
                     (isSpecialFormat && (symbol & 1) != 0)             //for PI number file
                        ? 0 : 1;
    }
    cout << endl;
    std::copy_n(epsilon.begin(), 100, std::ostream_iterator<BitSequence>(cout));
    cout << endl;

    inFile.close();

    return std::move(epsilon);
}

int generatorsTestConfigRun(int argc, char * argv[]) {
	time_t t;
    std::srand((unsigned int)(std::time(&t)));

    //-----------------------------Input data-----------------------------//

    int sizesStartIndex = 5;
    size_t len = (argc > 1) ? std::strlen(argv[1]) : 0;
    size_t countSizes = (argc >= sizesStartIndex + 1) ? std::atoi(argv[sizesStartIndex]) : 0;
    if (countSizes <= 0 || argc < int(sizesStartIndex + 1 + countSizes) || len < TEST_COUNT) {
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
   // std::vector<string> generatorNames { argv[4] };
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
//	for (int iGen = 0; iGen < generatorNames.size(); iGen++) {
        std::string genName = argv[4];

		//--------------------Container---------------------//

        uint32_t atom = //1u;
                1024u;

        const size_t TRAVERSAL_COUNT_LARGE = TRAVERSAL_COUNT_SMALL;
//        const size_t TRAVERSAL_THRESHOLD = size_t(1e5);
        Sequence epsilon;
        // TODO: Remove this crutch
		tp.n = 0;
        tp.numOfBitStreams = TRAVERSAL_COUNT_LARGE;
        tp.n = seqSizes[0] * atom;
        const size_t seqSize = tp.n * tp.numOfBitStreams;
//        for (auto jSize : seqSizes) {
//			tp.n += jSize * atom * ((jSize < TRAVERSAL_THRESHOLD) ? TRAVERSAL_COUNT_LARGE
//				: TRAVERSAL_COUNT_SMALL);
//		}

        auto genTimeExpend = my_get_current_clock_time();
		// TODO: Too much memory allocations
        std::normal_distribution<double> distribution(4.5, 2.0);		//doesn't failure with random_device generator
        //std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0)

        char genNameStr[80];
        strcpy(genNameStr, genName.c_str());
        char * inputFilename = strtok(genNameStr, "=");
        genName = inputFilename;

        // ------------------------- Output Results ----------------------------- //
        std::ofstream resFile;
        string outFilename = "resStdGenerators_" + genName;
        for (auto iSize : seqSizes)
            outFilename += "_" + std::to_string(iSize);
        resFile.open(outFilename + ".dat", std::ios::out | std::ios::trunc);
        cout << outFilename << endl;
        // Extra data (p_values)
//        std::ofstream extraFile;
//        extraFile.open("extraStdGenerators_" + genName + "_" + std::to_string(firstSize)
//                       + "-" + std::to_string(lastSize) + ".dat",
//            std::ios::out | std::ios::trunc);

        if ("file" == genName) {
            char * isBitRead = strtok(NULL, "=");
            inputFilename = strtok(NULL, "=");
            if (strcmp(isBitRead, "2") == 0)
                epsilon = readSequenceByBitFromFile(inputFilename, seqSize);
            else
                epsilon = readSequenceByByteFromFile(inputFilename, seqSize);
        }
        else if ("minstd_rand" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorMinstdRand, &distribution]() -> bool {
                return (int(std::round(distribution(generatorMinstdRand))) % inputOppositePossibility == 0);
            });
        }
        else if ("minstd_rand0" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorMinstdRand0, &distribution]() -> bool {
                return (int(std::round(distribution(generatorMinstdRand0))) % inputOppositePossibility == 0);
            });
        }
        else if ("knuth_b" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorKnuthB, &distribution]() -> bool {
                return (int(std::round(distribution(generatorKnuthB))) % inputOppositePossibility == 0);
            });
        }
        else if ("ranlux24" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorRanlux24, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRanlux24))) % inputOppositePossibility == 0);
            });
        }
        else if ("ranlux48" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorRanlux48, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRanlux48))) % inputOppositePossibility == 0);
            });
        }
        else if ("random_device" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorRandomDevice, &distribution]() -> bool {
                return (int(std::round(distribution(generatorRandomDevice))) % inputOppositePossibility == 0);
            });
        }
        else if ("rand" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorDefaultRandomEngine, &distribution]() -> bool {
                return ((std::rand() % 2) == 0);
            });
        }
        else if ("mersenne" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &generatorMt19937_64, &distribution]() -> bool {
                return (int(std::round(distribution(generatorMt19937_64))) % inputOppositePossibility == 0);
            });
        }
        else if ("bad" == genName) {
            std::generate_n(std::back_inserter(epsilon), seqSize,
                [&inputOppositePossibility, &distribution]() -> bool {
                static int i = 0;
                return (i++ < int(tp.n));
            });
        }
        else if ("lcg" == genName)
			epsilon = lcg();
        else if ("SHA1" == genName) { //wrong given results
            epsilon = SHA1();
        }
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

        {
        cout << "Generator time expend: "
             << getTimeDifferenceInMillis(genTimeExpend, my_get_current_clock_time()) / 1000
             << " secs." << endl;
        }

		size_t accumulatorSize = 0u;
        for (auto & iSize : seqSizes) {
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
            //cout << seqSizes.size() << " " << traversalCount << " " << generatorNames.size() << endl;

#pragma omp parallel for private(currResults) shared(genName, traversalCount, testKey, testResults)
			for (int jTraver = 0; jTraver < traversalCount; jTraver++) 
            {
                {
                    if (jTraver % 10 == 0)
                        cout << "(INFO) Traversal: " << jTraver << endl;
                }
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

//                {
//                    auto iterEnd = epsilonRange.begin();
//                    std::advance(iterEnd, 20);
//                    std::copy(epsilonRange.begin(), iterEnd,
//                        std::ostream_iterator<bool>(cout, ""));
//                    std::cout << std::endl;
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
                                return ((p_value < 0.) ? (count - 1000.) : ((p_value < MEANING_LEVEL) + count)); }
                        // p_value < ALPHA - it is failure
                        );
                    }
                    currResults.clear();
                }
			}

			//----------------Write results-----------------//
            {
                if (&iSize == &seqSizes.front()) {
                    resFile << "\t";
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
                    resFile << endl;
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(cout, "\t"));
                    cout << endl;
                }
                resFile << iSize << "_Kbits\t";
                std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"));
                resFile << endl;
                std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(cout, "\t"));
                cout << endl;
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
//	}

    cout << endl << "Whole time expend: "
         << getTimeDifferenceInMillis(wholeTimeExpend, my_get_current_clock_time()) / 1000
         << "sec." << endl;
	

	return 0;
}

}

//        std::ifstream inData;
//        //linux
//        inData.open("data/data.fourierExample", std::ios::in);
//        //windows
//        //inData.open("data\\data.fourierExample", std::ios::in);
//        inData.seekg(0);
//        for (auto & elem : epsilon) {
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

//                    std::ofstream outFile;
//                    string inputFile = "genSequences/kek/" + genName + "_" + std::to_string(iSize)
//                                 + "_" + std::to_string(jTraver);
//                    outFile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
//                    outFile.open(inputFile, std::ios::trunc);
//                    auto outIter = std::ostream_iterator<char>(outFile);
//                    int bitPos = 0;
//                    char buffer = 0;
//                    for (auto iter = epsilonRange.begin(); iter != epsilonRange.end(); iter++) {
//                        buffer |= (*iter) << (bitPos++);
//                        if (bitPos >= 8) {
//                            bitPos = 0;
//                            *(outIter++) = buffer;
//                            buffer = 0;
//                        }
//                    }
//                    if (bitPos > 0)
//                        *(outIter) = buffer;
//                    outFile.close();

