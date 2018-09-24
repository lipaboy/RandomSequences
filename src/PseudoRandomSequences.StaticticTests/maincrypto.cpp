#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <functional>

#include "statTests/include/generators.h"

using namespace statistical_tests_space;

using std::cout;
using std::endl;

#include <boost/range/any_range.hpp>

using AnyRange = boost::any_range<int, boost::forward_traversal_tag, int, std::ptrdiff_t>;

int main(int argc, char *argv[]) {

//    auto res = sha256("0");
//    cout << res << endl;

    cout << "Tests the tests: ";
    cout << runUnitTests() << endl;
    cout << "Run tests: " << endl;
    return generatorsTestConfigRun(argc, argv);

}

