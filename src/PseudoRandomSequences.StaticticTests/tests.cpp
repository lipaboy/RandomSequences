#include "pseudoRandomSequences.h"
#include "statTests/include/generators.h"
#include <lipaboyLibrary/src/maths/fixed_precision_number.h>

namespace statistical_tests_space {

using std::cout;
using std::endl;
using std::vector;
using std::string;
using LipaboyLib::FixedPrecisionNumber;

bool runUnitTests() {
    const size_t SIZE = 1000000;
    auto epsilon = readSequenceByByteFromFile("data/data.e", SIZE);
    bool isPassed = true;

    FrequencyTest freqTest;
    auto res = freqTest.test(epsilon.begin(), epsilon.size());
    isPassed = isPassed && (FixedPrecisionNumber<double, int, 1, -6>(res.front()) == 0.953749);

    return isPassed;
}

}
