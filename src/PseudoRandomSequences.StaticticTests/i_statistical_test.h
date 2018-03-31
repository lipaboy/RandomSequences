#ifndef I_STATISTICAL_TEST_H
#define I_STATISTICAL_TEST_H

#include <vector>

#include "statTests/include/stat_fncs.h"
#include "bookStackTest/order_test.h"

namespace statistical_tests_space {

using std::vector;

// TODO: test must have the additional parameters (except length of sequence)
class IStatisticalTest {
public:
    using PValueType = double;
    using ReturnValueType = vector<PValueType>;
    using size_type = size_t;

public:
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) = 0;
    // TODO: use iterator_traits
    ReturnValueType test(BoolIterator sequenceIterBegin, BoolIterator sequenceIterEnd) {
        return test(sequenceIterBegin, std::distance(sequenceIterBegin, sequenceIterEnd));
    }
};

class TestParameters;

class FrequencyTest : public IStatisticalTest {
public:
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size) {
        return ReturnValueType(Frequency(size, sequenceIter));
    }
};

class BlockFrequencyTest : public IStatisticalTest {
public:
    virtual ReturnValueType test(BoolIterator sequenceIter, size_type size);
};

class TestParameters {
public:
    TestParameters(size_t);

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
