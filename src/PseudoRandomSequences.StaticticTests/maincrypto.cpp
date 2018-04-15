#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <boost/dynamic_bitset.hpp>

using namespace statistical_tests_space;

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    //return generatorsTestConfigRun(argc, argv);
    cout << runUnitTests() << endl;
}

