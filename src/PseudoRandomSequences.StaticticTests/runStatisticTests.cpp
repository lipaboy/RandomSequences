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

    // TODO: make perfomance test on removing temporary files in BookStackTest

IStatisticalTest::ReturnValueType
statistical_tests_space::runStatisticalTests(BoolIterator epsilonBegin,
    BoolIterator epsilonEnd,
    std::string const & testKey,
    IStatisticalTest::TestParametersPtr pTestParams)
{
    IStatisticalTest::ReturnValueType testResults;
    const auto EPSILON_SIZE = std::distance(epsilonBegin, epsilonEnd);
    TestsContainerType packOfTests;

    packOfTests.emplace_back(make_unique<BookStackTest>(pTestParams));
    packOfTests.emplace_back(make_unique<FrequencyTest>(pTestParams));
    packOfTests.emplace_back(make_unique<BlockFrequencyTest>(pTestParams));
    packOfTests.emplace_back(make_unique<RunsTest>(pTestParams));
    packOfTests.emplace_back(make_unique<LongestRunOfOnesTest>(pTestParams));
    packOfTests.emplace_back(make_unique<RankTest>(pTestParams));
    packOfTests.emplace_back(make_unique<DiscreteFourierTransformTest>(pTestParams));
    packOfTests.emplace_back(make_unique<NonOverlappingTemplateMatchingsTest>(pTestParams));
    packOfTests.emplace_back(make_unique<OverlappingTemplateMatchingsTest>(pTestParams));
    packOfTests.emplace_back(make_unique<UniversalTest>(pTestParams));
    packOfTests.emplace_back(make_unique<SerialTest>(pTestParams));
    packOfTests.emplace_back(make_unique<ApproximateEntropyTest>(pTestParams));
    packOfTests.emplace_back(make_unique<CumulativeSumsTest>(pTestParams));
    packOfTests.emplace_back(make_unique<RandomExcursionsTest>(pTestParams));
    packOfTests.emplace_back(make_unique<RandomExcursionsVariantTest>(pTestParams));
    packOfTests.emplace_back(make_unique<OrderTest>(pTestParams));
    // TODO: check on discreditation (maybe only parameter 8 doesn't work)
    packOfTests.emplace_back(make_unique<LinearComplexityTest>());    // test doesn't work again

    size_t charPos = 0;
    for (auto & testObj : packOfTests) {
        if (testKey[charPos++] == '1') {
            auto res = testObj->test(epsilonBegin, static_cast<size_t>(EPSILON_SIZE));
            for (auto elem : res)
                testResults.push_back(elem);
        }
    }

    return testResults;
}

// TODO: join TestWrappers with their names
vector<string> statistical_tests_space::getStatisticTestNames(string testKey, size_t sequenceSize) {
    using std::to_string;

    vector<string> testNames;
    TestParameters testParameters;

    size_t ind = 0;
    if (testKey[ind++] == '1')
        for (auto param : testParameters.bookStackTest)
            testNames.push_back("BookStackTest_" + std::to_string(param.dimension)
                                + "_dim_" + std::to_string(param.upperPart) + "_up");
    if (testKey[ind++] == '1')
        testNames.push_back("Frequency");
    if (testKey[ind++] == '1')
        for (auto blockSize : testParameters.blockFrequencyTest)
            testNames.push_back("BlockFrequency_" + std::to_string(blockSize));
    if (testKey[ind++] == '1')
        testNames.push_back("Runs");;
    if (testKey[ind++] == '1')
        testNames.push_back("LongestRunOfOnes");
    if (testKey[ind++] == '1')
        testNames.push_back("Rank");
    if (testKey[ind++] == '1')
        testNames.push_back("DiscreteFourierTransform");
    if (testKey[ind++] == '1')
        for (auto param : testParameters.nonOverlappingTemplateMatchingsTest)
            testNames.push_back("NonOverlappingTemplateMatchings_"
                + std::to_string(param));
    if (testKey[ind++] == '1')
        for (auto param : testParameters.overlappingTemplateMatchingsTest)
            testNames.push_back("OverlappingTemplateMatchings_" + std::to_string(param));
    if (testKey[ind++] == '1')
        testNames.push_back("Universal");
    if (testKey[ind++] == '1')
        for (auto param : testParameters.serialTest) {
            testNames.push_back("Serial_" + std::to_string(param) + "_1");
            testNames.push_back("Serial_" + std::to_string(param) + "_2");
        }
    if (testKey[ind++] == '1')
        for (auto param : testParameters.approximateEntropyTest)
            testNames.push_back("ApproximateEntropy_" + std::to_string(param));
    if (testKey[ind++] == '1') {
        testNames.push_back("CumulativeSums_1");
        testNames.push_back("CumulativeSums_2");
    }
    if (testKey[ind++] == '1')
        testNames.push_back("RandomExcursions");
    if (testKey[ind++] == '1')
        testNames.push_back("RandomExcursionsVariant");
    if (testKey[ind++] == '1')
        for (auto param : testParameters.orderTest)
            testNames.push_back("OrderTest_" + std::to_string(param.dimension)
                                + "_dim_" + std::to_string(param.upperPart) + "_up");
    if (testKey[ind++] == '1')
        for (auto param : testParameters.linearComplexityTest)
            testNames.push_back("LinearComplexity_" + std::to_string(param));

    return testNames;
}


//--------------------Time measuring-----------------//

TimeType statistical_tests_space::getCurrentClockTime() {
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
   return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(to - from).count());
#elif _WIN32   //Windows
   return to - from;
#endif
}


