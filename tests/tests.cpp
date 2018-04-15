#include <iostream>
#include <vector>
#include <cstdint>

#include <iterator>
#include <unordered_map>
#include <string>
#include <utility>
#include <memory>

#include <gtest/gtest.h>

//#include "i_statistical_test.h"

//#include <pseudoRandomSequences.h>
//#include "statTests/include/generators.h"
//#include <lipaboyLibrary/src/maths/fixed_precision_number.h>
//#include <statTests/include/externs.h>

namespace random_sequences_tests {

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;


//extern Sequence readSequenceByByteFromFile(string const & inputFile, size_t sequenceSize,
//                                           char isZero, bool isSpecialFormat);

//---------------------------------Tests-------------------------------//

//---------Constructors---------//

TEST(OrderTest_test, check_method_test) {

    ASSERT_EQ(1, 1);
}

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
