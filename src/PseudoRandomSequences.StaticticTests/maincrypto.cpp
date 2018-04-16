#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>

using namespace statistical_tests_space;

using std::cout;
using std::endl;

#include <boost/range/any_range.hpp>

using AnyRange = boost::any_range<int, boost::forward_traversal_tag, int, std::ptrdiff_t>;

int main(int argc, char *argv[]) {
    //return generatorsTestConfigRun(argc, argv);
    //cout << runUnitTests() << endl;

    std::vector<int> kek({1, 2, 3, 4, 5, 6});
    AnyRange range = kek;
    range.advance_begin(4);
}

