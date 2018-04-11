#ifndef I_STATISTICAL_TEST_H
#define I_STATISTICAL_TEST_H

#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <iterator>
#include <omp.h>
#include <memory>

#include "statTests/include/externs.h"
#include "statTests/include/stat_fncs.h"
#include "bookStackTest/order_test.h"

namespace statistical_tests_space {

using std::vector;
using std::string;
using std::shared_ptr;

const int TEST_COUNT = 16;
const double MEANING_LEVEL = 0.05;

inline bool isTestSuccessful(double pValueOfTest) {
    return (pValueOfTest >= MEANING_LEVEL);
}

class TestParameters;

// TODO: test must have the additional parameters (except length of sequence)
class IStatisticalTest {
public:
    using PValueType = double;
    using ReturnValueType = vector<PValueType>;
    using size_type = size_t;
    using TestParametersPtr = shared_ptr<TestParameters>;

public:
    IStatisticalTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : pTestParams_(pTestParams) {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) = 0;
    // TODO: use iterator_traits
    ReturnValueType test(BoolIterator sequenceIterBegin, BoolIterator sequenceIterEnd) {
        return test(sequenceIterBegin, std::distance(sequenceIterBegin, sequenceIterEnd));
    }

    const TestParameters& getTestParameters() const { return *pTestParams_; }

private:
    TestParametersPtr pTestParams_;
};

class FrequencyTest : public IStatisticalTest {
public:
    FrequencyTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { statistical_tests_space::doFrequencyTest(int(size), sequenceIter) } );
    }
};
// #Parameterized
class BlockFrequencyTest : public IStatisticalTest {
public:
    BlockFrequencyTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class RunsTest : public IStatisticalTest {
public:
    RunsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { doRunsTest(size, sequenceIter) } );
    }
};

class LongestRunOfOnesTest : public IStatisticalTest {
public:
    LongestRunOfOnesTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { doLongestRunOfOnesTest(size, sequenceIter) } );
    }
};

class RankTest : public IStatisticalTest {
public:
    RankTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { doRankTest(size, sequenceIter) } );
    }
};

class DiscreteFourierTransformTest : public IStatisticalTest {
public:
    DiscreteFourierTransformTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}

    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { statistical_tests_space::doDiscreteFourierTransformTest(size, sequenceIter) } );
    }
};
// #The Slowest test
// #Parameterized
//2 - is minimum (depends on existing files)
class NonOverlappingTemplateMatchingsTest : public IStatisticalTest {
public:
    NonOverlappingTemplateMatchingsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Parameterized
class OverlappingTemplateMatchingsTest : public IStatisticalTest {
public:
    OverlappingTemplateMatchingsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class UniversalTest : public IStatisticalTest {
public:
    UniversalTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType( { doUniversalTest(size, sequenceIter) } );
    }
};
// #The Slowest test
// #Parameterized
// #Wrong test. Excluded from the pack of tests. (Reason why: discredits the random sequences)
class LinearComplexityTest : public IStatisticalTest {
public:
    LinearComplexityTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Slow (time grows very much)
// #Parameterized
class SerialTest : public IStatisticalTest {
public:
    SerialTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Parameterized
class ApproximateEntropyTest : public IStatisticalTest {
public:
    ApproximateEntropyTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class CumulativeSumsTest : public IStatisticalTest {
public:
    CumulativeSumsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class RandomExcursionsTest : public IStatisticalTest {
public:
    RandomExcursionsTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class RandomExcursionsVariantTest : public IStatisticalTest {
public:
    RandomExcursionsVariantTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};
// #Slow in case where sequence is short
// #Parameterized
class BookStackTest : public IStatisticalTest {
public:
    BookStackTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>()) : IStatisticalTest(pTestParams) {}
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

//----------------------Test parameters----------------------//

class TestParameters {
public:
    TestParameters();

    struct BookStackPair { uint64_t upperPart; uint64_t dimension; };
    vector<BookStackPair> bookStackTest;

    vector<uint64_t> blockFrequencyTest;
    vector<uint64_t> nonOverlappingTemplateMatchingsTest;
    vector<uint64_t> overlappingTemplateMatchingsTest;		// TODO: CHECK
    vector<uint64_t> linearComplexityTest;	// must be > 3
    vector<uint64_t> serialTest;
    vector<uint64_t> approximateEntropyTest;
};



}

#endif // I_STATISTICAL_TEST_H
