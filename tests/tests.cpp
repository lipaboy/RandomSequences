#include <iostream>
#include <vector>
#include <cstdint>
#include <iterator>
#include <string>

#include <gtest/gtest.h>

#include "statistical_tests/pseudo_random_sequences.h"
#include "statistical_tests/statTests/include/externs.h"

//#include "nist_statistical_tests/statTests/include/stat_fncs.h"
//#include "nist_statistical_tests/statTests/include/externs.h"
//#include "nist_statistical_tests/statTests/include/decls.h"
//#include "nist_statistical_tests/statTests/include/cephes.h"  
//#include "nist_statistical_tests/statTests/include/utilities.h"

//#include "statistical_tests/lipaboyLibrary/src/maths/fixed_precision_number.h"
//#include "statistical_tests/lipaboyLibrary/src/intervals/segment.h"

namespace random_sequences_tests {

using std::cout;
using std::endl;
using std::vector;
using std::string;

//using lipaboy_lib::FixedPrecisionNumber;
using statistical_tests_space::doFrequencyTest;
using statistical_tests_space::Sequence;
using statistical_tests_space::readSequenceByByteFromFile;

//---------------------------------Tests-------------------------------//

//---------Constructors---------//

TEST(Check, check) {
		//using DoubleComparisionType = FixedPrecisionNumber<double, int, 1, -6>;
		// windows operation system is slower than linux (too big size)
		const size_t SIZE = 10000;

		Sequence sequence = readSequenceByByteFromFile("data/data.e", SIZE, '0', false);

		double res = doFrequencyTest(sequence.size(), sequence.begin());

		/*tp.n = SIZE;
		tp.numOfBitStreams = 1;
		
		auto p_value = Frequency(tp.n);*/

		//ASSERT_TRUE(DoubleComparisionType(res) == p_value);
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
