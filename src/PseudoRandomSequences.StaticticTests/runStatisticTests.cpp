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
#include <memory>

#include <omp.h>

#include "lipaboyLibrary/src/maths/fixed_precision_number.h"

using namespace statistical_tests_space;


using std::vector;
using std::pair;
using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;

using TestsContainerType = vector<unique_ptr<IStatisticalTest> >;

    // TODO: take off the responsibility of return names of active tests and give it to another function with argument testKey

void statistical_tests_space::runStatisticalTests(
	BoolIterator epsilonBegin,
    BoolIterator epsilonEnd,
	std::vector<double> & testResults,
    std::string const & testKey,
    std::string const & uniqueSequenceName
        )
{
    const int EPSILON_SIZE = static_cast<int>(std::distance(epsilonBegin, epsilonEnd));

    TestsContainerType packOfTests;
    packOfTests.emplace_back(make_unique<BookStackTest>());
    packOfTests.emplace_back(make_unique<FrequencyTest>());
    packOfTests.emplace_back(make_unique<BlockFrequencyTest>());
    packOfTests.emplace_back(make_unique<RunsTest>());
    packOfTests.emplace_back(make_unique<LongestRunOfOnesTest>());
    packOfTests.emplace_back(make_unique<RankTest>());
    packOfTests.emplace_back(make_unique<DiscreteFourierTransformTest>());
    packOfTests.emplace_back(make_unique<NonOverlappingTemplateMatchingsTest>());
    packOfTests.emplace_back(make_unique<OverlappingTemplateMatchingsTest>());
    packOfTests.emplace_back(make_unique<UniversalTest>());
    // TODO: check on discreditation
//    packOfTests.emplace_back(make_unique<LinearComplexityTest>());    // test doesn't work again
    packOfTests.emplace_back(make_unique<SerialTest>());
    packOfTests.emplace_back(make_unique<ApproximateEntropyTest>());
    packOfTests.emplace_back(make_unique<CumulativeSumsTest>());
    packOfTests.emplace_back(make_unique<RandomExcursionsTest>());
    packOfTests.emplace_back(make_unique<RandomExcursionsVariantTest>());

    int charPos = 0;
    for (auto & testObj : packOfTests) {
        if (charPos == 10)
            charPos++;
        if (testKey[charPos++] == '1') {
            auto res = testObj->test(epsilonBegin, EPSILON_SIZE);
            for (auto & elem : res)
                testResults.push_back(elem);
        }
    }
}

// TODO: join TestWrappers with their names
vector<string> statistical_tests_space::getStatisticTestNames(string testKey, size_t sequenceSize) {
    vector<string> testNames;
    TestParameters testParameters(sequenceSize);

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
//    if (testKey[10] == '1')
//        for (auto param : testParameters.linearComplexityTest)
//            testNames.push_back("LinearComplexity_" + std::to_string(param));
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


//--------------------Time measuring-----------------//

TimeType statistical_tests_space::my_get_current_clock_time() {
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

int statistical_tests_space::getTimeDifferenceInMillis(TimeType const & from, TimeType const & to) {
#ifdef __linux__
   return std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count();
#elif _WIN32   //Windows
   return to - from;
#endif
}

