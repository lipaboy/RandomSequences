#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <boost/dynamic_bitset.hpp>

using namespace PseudoRandomSequences;

int main(int argc, char *argv[]) {
//    return beaconRun(argc, argv);
//    return generatorsTestConfigRun(argc, argv);

    OrderTest orderTest;
    orderTest.initialize(1);
    Sequence seq = { 0, 1, 0, 1, 1, 0, 0 };
    orderTest.test(seq.begin(), seq.end());
}

