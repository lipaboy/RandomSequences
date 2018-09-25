#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <iterator>
#include <memory>

#include "i_statistical_test.h"
#include "statTests/include/stat_fncs.h"
#include "bookStackTest/order_test.h"

namespace statistical_tests_space {

using std::vector;
using std::string;
using std::shared_ptr;

const int TEST_COUNT = 17;

class FrequencyTest : public IStatisticalTest {
public:
    FrequencyTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
//    virtual ~FrequencyTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { parseTestResult(doFrequencyTest(int(size), sequenceIter)) } );
    }
};
// #Parameterized
class BlockFrequencyTest : public IStatisticalTest {
public:
    BlockFrequencyTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~BlockFrequencyTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class RunsTest : public IStatisticalTest {
public:
    RunsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~RunsTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { parseTestResult(doRunsTest(int(size), sequenceIter)) } );
    }
};

class LongestRunOfOnesTest : public IStatisticalTest {
public:
    LongestRunOfOnesTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~LongestRunOfOnesTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { parseTestResult(doLongestRunOfOnesTest(size, sequenceIter)) } );
    }
};

class RankTest : public IStatisticalTest {
public:
    RankTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~RankTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { parseTestResult(doRankTest(size, sequenceIter)) } );
    }
};

class DiscreteFourierTransformTest : public IStatisticalTest {
public:
    DiscreteFourierTransformTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~DiscreteFourierTransformTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { parseTestResult(doDiscreteFourierTransformTest(size, sequenceIter)) } );
    }
};
// #The Slowest test
// #Parameterized
//2 - is minimum (depends on existing files)
class NonOverlappingTemplateMatchingsTest : public IStatisticalTest {
public:
    NonOverlappingTemplateMatchingsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~NonOverlappingTemplateMatchingsTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Parameterized
class OverlappingTemplateMatchingsTest : public IStatisticalTest {
public:
    OverlappingTemplateMatchingsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~OverlappingTemplateMatchingsTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class UniversalTest : public IStatisticalTest {
public:
    UniversalTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~UniversalTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { parseTestResult(doUniversalTest(size, sequenceIter)) } );
    }
};
// #The Slowest test
// #Parameterized
// #Wrong test. Excluded from the pack of tests. (Reason why: discredits the random sequences)
class LinearComplexityTest : public IStatisticalTest {
public:
    LinearComplexityTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~LinearComplexityTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Slow (time grows very much)
// #Parameterized
class SerialTest : public IStatisticalTest {
public:
    SerialTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~SerialTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Parameterized
class ApproximateEntropyTest : public IStatisticalTest {
public:
    ApproximateEntropyTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~ApproximateEntropyTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class CumulativeSumsTest : public IStatisticalTest {
public:
    CumulativeSumsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~CumulativeSumsTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class RandomExcursionsTest : public IStatisticalTest {
public:
    RandomExcursionsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~RandomExcursionsTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class RandomExcursionsVariantTest : public IStatisticalTest {
public:
    RandomExcursionsVariantTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~RandomExcursionsVariantTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Slow in case where sequence is short
// #Parameterized
class BookStackTest : public IStatisticalTest {
public:
    BookStackTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : IStatisticalTest(pTestParams) {}
    virtual ~BookStackTest() {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};



}

