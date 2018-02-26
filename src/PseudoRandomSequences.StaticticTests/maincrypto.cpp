#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <boost/dynamic_bitset.hpp>

#include "ListOfVectors.h"

using namespace PseudoRandomSequences;
using namespace LipaboyLib;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
	
//    return beaconRun(argc, argv);
    //return generatorsTestConfigRun(argc, argv);
    ListOfVectors<int> list(5, 5);
    list[10] = 2;
    cout << list[10];
}

