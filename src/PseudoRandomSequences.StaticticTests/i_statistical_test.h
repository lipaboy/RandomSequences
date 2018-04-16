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

const int TEST_COUNT = 16;
const double MEANING_LEVEL = 0.05;

inline bool isTestSuccessful(double pValueOfTest) {
    return (pValueOfTest >= MEANING_LEVEL);
}

// TODO: make interface of TestParameters
class TestParameters;

// TODO: test must have the additional parameters (except length of sequence)
class IStatisticalTest {
public:
    using PValueType = double;
    using ReturnValueType = vector<PValueType>;
    using size_type = size_t;
    using TestParametersPtr = shared_ptr<TestParameters>;

public:
    IStatisticalTest(TestParametersPtr pTestParams = std::make_shared<TestParameters>())
        : pTestParams_(pTestParams) {}

    // Make methods more common (template for example)
    virtual ReturnValueType test(BoolIterator sequenceIterBegin, size_type size) = 0;

    ReturnValueType test(BoolIterator sequenceIterBegin, BoolIterator sequenceIterEnd) {
        return test(sequenceIterBegin, std::distance(sequenceIterBegin, sequenceIterEnd));
    }

    const TestParameters& getTestParameters() const { return *pTestParams_; }

private:
    TestParametersPtr pTestParams_;
};

}

#endif // I_STATISTICAL_TEST_H
