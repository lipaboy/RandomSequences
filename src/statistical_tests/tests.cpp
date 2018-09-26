#include "pseudo_random_sequences.h"
#include "statTests/include/generators.h"

#include <lipaboy_library/src/maths/fixed_precision_number.h>
#include <lipaboy_library/src/intervals/segment.h>

#include <fstream>

namespace statistical_tests_space {

// TODO: write unit tests for all the statistical ones
// TODO: write unit tests accomplish for Windows platform (by speed of execution)

using std::cout;
using std::endl;
using std::vector;
using std::string;
using LipaboyLib::FixedPrecisionNumber;

namespace {

string runUnitTestsForStatisticalTests() {
	string isPassed = "";

	try {
		using DoubleComparisionType = FixedPrecisionNumber<double, int, 1, -6>;
		// windows operation system is slower than linux (too big size)
		const size_t SIZE = 1000000;

		auto epsilon = readSequenceByByteFromFile("data/data.e", SIZE, '0', false);
		
		shared_ptr<TestParameters> parameters = std::make_shared<TestParameters>();

		parameters->approximateEntropyTest = { 10 };
		parameters->blockFrequencyTest = { 128 };
		parameters->nonOverlappingTemplateMatchingsTest = { 9 };
		parameters->overlappingTemplateMatchingsTest = { 9 };
		parameters->serialTest = { 16 };

		std::vector<double> res;

		//    res = BookStackTest(parameters).test(epsilon.begin(), epsilon.size());
		//    isPassed = isPassed && (DoubleComparisionType(res.front()) == 0.953749);
		res = { doFrequencyTest(epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.953749);
		res = { doBlockFrequencyTest(128, epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.211072);
		res = { doRunsTest(epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.561917);
		res = { doLongestRunOfOnesTest(epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.718945);
		res = { doRankTest(epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.306156);
		res = { doDiscreteFourierTransformTest(epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.847187);
		//    res = { doNonOverlappingTemplateMatchingsTest(9, epsilon.size(), epsilon.begin()) };
		//    isPassed = isPassed && (DoubleComparisionType(res.front()) == 0.953749);
		res = { doOverlappingTemplateMatchingsTest(9, epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.110434);
		res = { doUniversalTest(epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.282568);
		auto pair = doSerialTest(16, epsilon.size(), epsilon.begin());
		isPassed = isPassed + std::to_string(DoubleComparisionType(pair.first) == 0.766182
			&& DoubleComparisionType(pair.second) == 0.462921);
		res = { doApproximateEntropyTest(10, epsilon.size(), epsilon.begin()) };
		isPassed = isPassed + std::to_string(DoubleComparisionType(res.front()) == 0.700073);
		pair = doCumulativeSums(epsilon.size(), epsilon.begin());
		isPassed = isPassed + std::to_string(DoubleComparisionType(pair.first) == 0.669886
			&& DoubleComparisionType(pair.second) == 0.724265);
		//    res = { doRandomExcursionsTest(epsilon.size(), epsilon.begin()) };
		//    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.953749);
		//    res = { doRandomExcursionsVariantTest(epsilon.size(), epsilon.begin()) };
		//    isPassed = isPassed + std::to_string (DoubleComparisionType(res.front()) == 0.953749);

	}
	catch (std::ifstream::failure e) {
		return "";
	}
    return isPassed;
}

string runUnitTestsForOrderTest() {
    OrderTest orderTest;
    orderTest.initialize(2, 1);

    size_t partSize = 1000;

    Sequence seq  = readSequenceByBitFromFile("data/data.sha1", 10 * partSize);



    //------------------For debugging----------------//
//    tp.n = 1000;
//    tp.numOfBitStreams = 1;
    //statistical_tests_space::quadRes1();
    //readSequenceByByteFromFile("data/data.pi", 10000, '0', false);
//    std::generate_n(std::back_inserter(seq), 1000,
//                    [] () -> bool {
//                        static int i = -1;
//                        i++;
//                        return (LipaboyLib::Segment<int>(1, 2).in(i % 4)) ? 1 : 0;
//                    });
    //std::copy(seq.begin(), seq.end(), std::ostream_iterator<bool>(cout));
    cout << endl;
    auto startTime = getCurrentClockTime();
    int counterFailure = 0;
    for (int i = 0; i < 10; i++) {
        auto iter = seq.begin();
        std::advance(iter, i * partSize);
        auto pValue = orderTest.test(iter, partSize);

        counterFailure += !parseTestResult(pValue.front());
//        cout << "Order test result: " << isTestSuccessful(pValue.front()) << ", " << pValue.front()
//             << endl
//             << "Time elapsed: " << getTimeDifferenceInMillis(startTime, getCurrentClockTime())
//             << endl;
    }
    cout << counterFailure << endl;

    return "";
}

}

string runUnitTests() {

    string isPassed = "";

    isPassed += runUnitTestsForStatisticalTests();
//    isPassed += runUnitTestsForOrderTest();


    return isPassed;
}

}
