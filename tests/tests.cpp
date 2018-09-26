#include <iostream>
#include <vector>
#include <cstdint>
#include <iterator>
#include <string>

#include <gtest/gtest.h>

#include "statistical_tests/pseudo_random_sequences.h"
#include "nist_statistical_tests/statTests/include/stat_fncs.h"

#include "statistical_tests/lipaboy_library/src/maths/fixed_precision_number.h"
#include "statistical_tests/lipaboy_library/src/intervals/segment.h"

namespace random_sequences_tests {

using std::cout;
using std::endl;
using std::vector;
using std::string;

using LipaboyLib


//extern Sequence readSequenceByByteFromFile(string const & inputFile, size_t sequenceSize,
//                                           char isZero, bool isSpecialFormat);

//---------------------------------Tests-------------------------------//

//---------Constructors---------//

TEST(Check, check) {
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
		Frequency(5);

		ASSERT_EQ(1, 1);
	}
	catch (std::ifstream::failure e) {

	}
}

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
	auto res = RUN_ALL_TESTS();
#if(defined _MSC_VER) || (defined __BORLANDC__) || (defined _WIN32)
	system("pause");
#endif
    return res;
}
