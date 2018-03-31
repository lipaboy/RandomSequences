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

namespace random_sequences_tests {

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;


//---------------------------------Tests-------------------------------//

//---------Constructors---------//

TEST(OrderTest, test) {

    ASSERT_EQ(1, 1);
}

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
