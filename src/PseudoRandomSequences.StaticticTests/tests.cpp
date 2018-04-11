#include "pseudoRandomSequences.h"
#include "statTests/include/generators.h"
#include <lipaboyLibrary/src/maths/fixed_precision_number.h>

namespace statistical_tests_space {

// TODO: write unit tests for all the statistical ones

using std::cout;
using std::endl;
using std::vector;
using std::string;
using LipaboyLib::FixedPrecisionNumber;

string runUnitTests() {
    using DoubleComparisionType = FixedPrecisionNumber<double, int, 1, -6>;
    const size_t SIZE = 1000000;
    auto epsilon = readSequenceByByteFromFile("data/data.e", SIZE);
    string isPassed = "";
    shared_ptr<TestParameters> parameters = std::make_shared<TestParameters>();

    parameters->approximateEntropyTest = {10};
    parameters->blockFrequencyTest = {128};
    parameters->nonOverlappingTemplateMatchingsTest = {9};
    parameters->overlappingTemplateMatchingsTest = {9};
    parameters->serialTest = {16};

    IStatisticalTest::ReturnValueType res;

//    res = BookStackTest(parameters).test(epsilon.begin(), epsilon.size());
//    isPassed = isPassed && (DoubleComparisionType(res.front()) == 0.953749);
    res = FrequencyTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.953749);
    res = BlockFrequencyTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.211072);
    res = RunsTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.561917);
    res = LongestRunOfOnesTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.718945);
    res = RankTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.306156);
    res = DiscreteFourierTransformTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.847187);
//    res = NonOverlappingTemplateMatchingsTest(parameters).test(epsilon.begin(), epsilon.size());
//    isPassed = isPassed && (DoubleComparisionType(res.front()) == 0.953749);
    res = OverlappingTemplateMatchingsTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.110434);
    res = UniversalTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.282568);
    res = SerialTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string (DoubleComparisionType(res[0]) == 0.766182
                                          && DoubleComparisionType(res[1]) == 0.462921 );
    res = ApproximateEntropyTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.700073);
    res = CumulativeSumsTest(parameters).test(epsilon.begin(), epsilon.size());
    isPassed = isPassed + std::to_string (DoubleComparisionType(res[0]) == 0.669886
                                          && DoubleComparisionType(res[1]) == 0.724265 );
//    res = RandomExcursionsTest(parameters).test(epsilon.begin(), epsilon.size());
//    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.953749);
//    res = RandomExcursionsVariantTest(parameters).test(epsilon.begin(), epsilon.size());
//    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.953749);


    return isPassed;
}

}
