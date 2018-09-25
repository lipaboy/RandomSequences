#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <functional>

#include "statTests/include/generators.h"

using namespace statistical_tests_space;

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {

//    auto res = sha256("0");
//    cout << res << endl;

    cout << "Tests the tests: ";
    //cout << runUnitTests() << endl;
    cout << "Run tests: " << endl;
    auto res = generatorsTestConfigRun(argc, argv);
#if(defined _MSC_VER) || (defined __BORLANDC__) || (defined _WIN32)
	system("pause");
#endif
	return res;
}

