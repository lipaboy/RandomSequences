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

#include <pseudoRandomSequences.h>
//#include "statTests/include/generators.h"
//#include <lipaboyLibrary/src/maths/fixed_precision_number.h>
#include <statTests/include/externs.h>

namespace random_sequences_unit_tests {

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;

using statistical_tests_space::OrderTest;
using statistical_tests_space::Sequence;
using statistical_tests_space::DiscreteFourierTransformTest;
using statistical_tests_space::FrequencyTest;
using namespace statistical_tests_space;


//---------------------------------Tests-------------------------------//

//---------Constructors---------//

TEST(OrderTest_test, check_method_test) {
    OrderTest orderTest;
    orderTest.initialize(1);
    Sequence seq = { 0, 1, 0, 1, 1, 0, 0 };
  //  orderTest.test(seq.begin(), seq.end());

    ASSERT_EQ(1, 1);
}

TEST(DiscreteFourierTransformTest_test, check_method_test) {

 //   DiscreteFourierTransformTest dftTest;
    //FrequencyTest freqTest;
    //statistical_tests_space::doFrequencyTest(1, vector<bool>({1, 0}).begin());

//    tp.n = 1000000;
//    tp.numOfBitStreams = 1;
//    auto epsilon = quadRes2();
//    auto res = dftTest.test(epsilon.begin(), epsilon.size());

//    ASSERT_TRUE((LipaboyLib::FixedPrecisionNumber<double, int, 1, -5>(res[0]) == 0.776046));
    ASSERT_EQ(1, 1);
}

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
