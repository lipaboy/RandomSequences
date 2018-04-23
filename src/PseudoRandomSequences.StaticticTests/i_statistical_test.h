#ifndef I_STATISTICAL_TEST_H
#define I_STATISTICAL_TEST_H

#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <iterator>
#include <memory>

#include "statTests/include/externs.h"

namespace statistical_tests_space {

using std::vector;
using std::string;
using std::shared_ptr;

// TODO: make interface of TestParameters
class TestParameters {
public:
    using size_type = size_t;
public:
    TestParameters();

    struct BookStackPair { size_type upperPart; size_type dimension; };
    vector<BookStackPair> bookStackTest;

    vector<size_type> blockFrequencyTest;
    vector<size_type> nonOverlappingTemplateMatchingsTest;
    vector<size_type> overlappingTemplateMatchingsTest;		// TODO: CHECK
    vector<size_type> linearComplexityTest;	// must be > 3
    vector<size_type> serialTest;
    vector<size_type> approximateEntropyTest;
    vector<BookStackPair> orderTest;
};

// TODO: test must have the additional parameters (except length of sequence)
class IStatisticalTest {
public:
    using PValueType = double;
    using TestResultType = bool;
    using ReturnValueType = vector<TestResultType>;
    using size_type = size_t;
    using TestParametersPtr = shared_ptr<TestParameters>;

public:
    IStatisticalTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : pTestParams_(pTestParams) {}

    // TODO: Make methods more common (template for example) (Big refactoring task)
    virtual ReturnValueType test(BoolIterator sequenceIterBegin, size_type size) = 0;

    ReturnValueType test(BoolIterator sequenceIterBegin, BoolIterator sequenceIterEnd) {
        return test(sequenceIterBegin, std::distance(sequenceIterBegin, sequenceIterEnd));
    }

    const TestParameters& getTestParameters() const { return *pTestParams_; }

private:
    TestParametersPtr pTestParams_;
};

//------------------------BitWordStream--------------------//

// Issue:
// Must return the bit words

// Note: You write the same in C++ course

// Input: all the types of stream (container, files, iterators)

template <typename WordType = unsigned int,
          class TIterator = typename std::vector<WordType>::iterator >
class BitWordStream {
public:
    using iterator = TIterator;

public:

//    WordType operator>> () const {
//        return 0u;
//    }

private:
    iterator begin_;
    iterator end_;
};

}

#endif // I_STATISTICAL_TEST_H




