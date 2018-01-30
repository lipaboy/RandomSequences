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

using std::string;
using std::vector;

// TODO: add parameter lambda function for some flushing the data into file

//return testNames, return results for each size
//vector<string>
ResultStructure
PseudoRandomSequences::testGenerator(string generatorName,
                                     vector<int> const & sequenceSizes,
                                     string const & testKey,
                                     int fieldModulusOfPossibility) {        // size of finite field that is used to choose randomly number
    time_t t;
    std::srand((unsigned int)(std::time(&t)));
    using std::cout;
    using std::endl;

    ResultStructure result;

    std::minstd_rand generatorMinstdRand;
    std::knuth_b generatorKnuthB;
    std::ranlux48 generatorRanlux48;	//failure with normal_distribution and with chi_squared_distribution
    std::random_device generatorRandomDevice;

    string & genName = generatorName;
    //--------------------Container---------------------//

    const int TRAVERSAL_COUNT_SMALL = 5;
    const int TRAVERSAL_COUNT_LARGE = TRAVERSAL_COUNT_SMALL;
    const size_t TRAVERSAL_THRESHOLD = size_t(1e5);
    Sequence epsilon;
    tp.n = 0;
    tp.numOfBitStreams = 1;
    for (int i = 0; i < sequenceSizes.size(); i++) {
        tp.n += sequenceSizes[i] * 1024u
                * ((sequenceSizes[i] < TRAVERSAL_THRESHOLD)
                   ? TRAVERSAL_COUNT_LARGE : TRAVERSAL_COUNT_SMALL);
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

    result.statTestNames = { "" };

    size_t accumulatorSize = 0u;
    for (int i = 0; i < sequenceSizes.size(); i++) {
        size_t inputSize = sequenceSizes[i];

        cout << ">-----------------------New Size-----------------------<" << endl;
        cout << endl << genName << endl;
        cout << "Sequence size = " << inputSize << endl;
        //-------------Input----------------//

        if (isStdGenerators)
            epsilon.resize(inputSize);

        std::string outFilename = "out.tmp";

        int traversalCount =
                (std::distance(epsilonRange.begin(), epsilonRange.end()) < TRAVERSAL_THRESHOLD)
                    ? TRAVERSAL_COUNT_LARGE : TRAVERSAL_COUNT_SMALL;

        result.pValues.emplace_back();
        result.resultsForEachSize.emplace_back();

        auto & pValues = result.pValues.back();
        auto & testResults = result.resultsForEachSize.back();

//#pragma omp parallel for reduction(+:accumulatorSize) private(epsilonRange) shared(genName, traversalCount)
        for (int iTraver = 0; iTraver < traversalCount; iTraver++)
        {
            // Generator factory
            {
                std::normal_distribution<double> distribution(4.5, 2.0);		//doesn't failure with random_device generator
                //std::chi_squared_distribution<double> distribution(3.0);		//failure with random_device (number of freedoms = 3.0)
                if ("minstd_rand" == genName) {
                    std::generate(epsilonRange.begin(), epsilonRange.end(),
                        [&fieldModulusOfPossibility, &generatorMinstdRand, &distribution, &inputSize]() -> bool {
                        return (int(std::round(distribution(generatorMinstdRand))) % fieldModulusOfPossibility == 0);
                    });
                }
                else if ("knuth_b" == genName) {
                    std::generate(epsilonRange.begin(), epsilonRange.end(),
                        [&fieldModulusOfPossibility, &generatorKnuthB, &distribution, &inputSize]() -> bool {
                        return (int(std::round(distribution(generatorKnuthB))) % fieldModulusOfPossibility == 0);
                    });
                }
                else if ("ranlux48" == genName) {
                    std::generate(epsilonRange.begin(), epsilonRange.end(),
                        [&fieldModulusOfPossibility, &generatorRanlux48, &distribution, &inputSize]() -> bool {
                        return (int(std::round(distribution(generatorRanlux48))) % fieldModulusOfPossibility == 0);
                    });
                }
                else if ("random_device" == genName) {
                    std::generate(epsilonRange.begin(), epsilonRange.end(),
                        [&fieldModulusOfPossibility, &generatorRandomDevice, &distribution, &inputSize]() -> bool {
                        return (int(std::round(distribution(generatorRandomDevice))) % fieldModulusOfPossibility == 0);
                    });
                }
                else {
                    auto iterBegin = epsilon.begin();
                    auto iterEnd = epsilon.begin();
                    std::advance(iterBegin, accumulatorSize);
                    std::advance(iterEnd, accumulatorSize + inputSize);
                    epsilonRange = MyBoolRange(iterBegin, iterEnd);
                    accumulatorSize
                            //vector<string> testNames = { "" };
                            += inputSize;
                }
            }

            // output in console to sure that sequences are different
            {
                auto iterEnd = epsilonRange.begin();
                std::advance(iterEnd, 10);
                std::copy(epsilonRange.begin(), iterEnd,
                    std::ostream_iterator<bool>(cout, ""));
                std::cout << std::endl;
            }

            //----------------Tests-----------------//
            {
                auto isTestFinishedLambda = [] (double pValue) { return std::abs(-1. - pValue) > 1e-5; }; // -1 means that test doesn't work
                auto isTestSuccessedLambda = [] (double pValue) { return (pValue >= ALPHA); };

                runTests(epsilonRange.begin(), epsilonRange.end(), result.statTestNames,
                    (i <= 0) && (iTraver <= 0), pValues, testKey);

                if (iTraver == 0)
                    testResults.assign(pValues.size(), 0);
                std::transform(pValues.begin(), pValues.end(), // first source
                    testResults.begin(),                                // second source
                    testResults.begin(),                                // destination
                    [&isTestFinishedLambda, &isTestSuccessedLambda](double p_value, double count) -> double {
                        return isTestFinishedLambda(p_value) ? count : !isTestSuccessedLambda + count; }
                );
            }
        }
    }

    return result;
}
