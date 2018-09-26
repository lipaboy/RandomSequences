#include "pseudoRandomSequences.h"

#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <fstream>

#include "statTests/include/generators.h"

using namespace statistical_tests_space;

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
	string filename;
	if (argc < 2)
		filename = "config.conf";
	else
		filename = argv[1];

	vector<string> arguments(1, "");
	std::ifstream inFile;
	inFile.open(filename, std::ios::in);
	while (!inFile.eof()) {
		string temp;
		inFile >> temp;
		if (temp == "")
			continue;
		arguments.push_back(std::move(temp));
	}
	inFile.close();

    cout << "Tests the tests: ";
    //cout << runUnitTests() << endl;

    cout << "Run tests: " << endl;
    auto res = generatorsTestConfigRun(arguments);

#if(defined _MSC_VER) || (defined __BORLANDC__) || (defined _WIN32)
	system("pause");
#endif
	return res;
}

