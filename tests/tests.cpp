#include <iostream>
#include <vector>
#include <cstdint>

#include <iterator>
#include <unordered_map>
#include <string>
#include <utility>
#include <memory>

#include <gtest/gtest.h>

#include <bookStackTest/order_test.h>
#include <statTests/include/externs.h>

namespace random_sequences_tests {

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;

using statistical_tests_space::OrderTest;
using statistical_tests_space::Sequence;


//---------------------------------Tests-------------------------------//

//---------Constructors---------//

TEST(OrderTest, check_method_test) {
    OrderTest orderTest;
    orderTest.initialize(1);
    Sequence seq = { 0, 1, 0, 1, 1, 0, 0 };
    orderTest.test(seq.begin(), seq.end());

    ASSERT_EQ(1, 1);
}

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
