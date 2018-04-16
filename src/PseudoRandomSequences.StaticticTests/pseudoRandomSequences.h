#ifndef PSEUDO_RANDOM_SEQUENCES_H
#define PSEUDO_RANDOM_SEQUENCES_H

#include <vector>
#include <bitset>
#include <chrono>

#ifdef __linux__
#include <unistd.h>
#include <sys/times.h>
#elif _WIN32
#include <time.h>
#endif

#include "nist_tests_wrapper.h"


namespace statistical_tests_space {

// LOCAL PLAN
// TODO: write wrapper around BitWords
// TODO: test generators by OrderTest

using std::vector;
using std::pair;
using std::string;
typedef size_t size_type;

#ifdef __linux__
    using std::chrono::time_point;
    typedef time_point<std::chrono::steady_clock> TimeType;
#elif _WIN32   //Windows
    typedef int TimeType;
#endif

// return in milliseconds
TimeType getCurrentClockTime();

int getTimeDifferenceInMillis(TimeType const & from, TimeType const & to);

Sequence readSequenceByBitFromFile(string const & inputFile, size_t sequenceSize);

// TODO: transfer the lambda to function to abstract the processing of data
Sequence readSequenceByByteFromFile(std::string const & inputFile, size_t sequenceSize,
                                           char isZero, bool isSpecialFormat);

vector<string> getStatisticTestNames(string testKey, size_t sequenceSize);

//--------------------------------------------------------------//

IStatisticalTest::ReturnValueType
runStatisticalTests(BoolIterator epsilonBegin,
    BoolIterator epsilonEnd,
    std::string const & testKey,
    IStatisticalTest::TestParametersPtr pTestParams);

int generatorsTestConfigRun(int argc, char * argv[]);

//----------------Tests------------------//

std::string runUnitTests();

}

#endif //PSEUDO_RANDOM_SEQUENCES_H
