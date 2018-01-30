#include "pseudoRandomSequences.h"

#include <iterator>
#include <bitset>
#include <string>
#include <array>
#include <cmath>
#include <random>

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>

#include "statTests/include/stat_fncs.h"
#include "lipaboyLibrary/src/maths/fixed_precision_number.h"
#include "statTests/include/generators.h"

using namespace PseudoRandomSequences;
using namespace std::chrono;

//TODO: try to use GoogleTests

typedef std::vector<bool> Sequence;

const int TEST_COUNT = 16;

namespace {
	class MyBoolRange {
	private:
		BoolIterator _begin, _end;
	public:
		MyBoolRange(BoolIterator beg, BoolIterator end) : _begin(beg), _end(end) {}
		BoolIterator const & begin() { return _begin; }
		BoolIterator const & end() { return _end; }
	};
}

int PseudoRandomSequences::generatorsTestConfigRun(int argc, char * argv[]) {
	time_t t;
    std::srand((unsigned int)(std::time(&t)));
	using std::string;
	using std::vector;
	using std::cout;
	using std::endl;


	size_t len = 0;
	if (argc < 4 || (len = std::strlen(argv[1])) < TEST_COUNT) {
		cout << "Not enough parameters ( testKey ("
			<< TEST_COUNT << ", current = " << len << "), "
			<< "input possibility, first size, last size)"
			<< endl;
		return -1;
	}

	// TODO: add test performance for conversation and tests

	// TODO: bad computation of input possibility (use another input format) (bad because 2/3)
	// Kbits
    int inputOppositePossibility = static_cast<int>(
        std::round(1.0 / boost::lexical_cast<double>(argv[2]))
        );
	size_t firstSize = boost::lexical_cast<size_t>(argv[3]);
	size_t lastSize = boost::lexical_cast<size_t>(argv[4]);
	std::vector<string> generatorNames{ 
		//"minstd_rand", 
			//"knuth_b", 
		//"ranlux48", 
			//"random_device",

        //---------------------------NIST generators----------------------------//

        "lcg",    //fast
        //"SHA1",   //fast
       // "exclusiveOR"   //fast
       // "micali_schnorr"    //fast

        //"modExp", //slow
        //"bbs",    //slow
	};

    string testKey(argv[1]);

    auto wholeTimeExpend = my_get_current_clock_time();
	for (int iGen = 0; iGen < generatorNames.size(); iGen++) {
		std::string & genName = generatorNames[iGen];
		
		std::ofstream resFile;
        std::ofstream extraFile;
		resFile.open("resStdGenerators_" + genName + "_" + std::to_string(firstSize) 
			+ "-" + std::to_string(lastSize) + ".dat",
			std::ios::out | std::ios::trunc);
        extraFile.open("extraStdGenerators_" + genName + ".dat",
            std::ios::out | std::ios::trunc);


        vector<int> sequenceSizes;
        size_t stepIterSize = 4;		// the step of size iteration (traversal step)
		for (size_t iSize = firstSize; iSize <= lastSize; iSize *= stepIterSize) {
			size_t inputSize = 1024u * iSize;
            sequenceSizes.push_back(inputSize);
        }

        ResultStructure result = testGenerator(genName, sequenceSizes, testKey, inputOppositePossibility);

        for (int i = 0; i < sequenceSizes.size(); i++) {
            int & iSize = sequenceSizes[i];
			//----------------Write results-----------------//
            auto & testNames = result.statTestNames;
            auto & pValues = result.pValues[i];
            auto & testResults = result.resultsForEachSize[i];
			{
                if (i <= 0) {
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(resFile, "\t"));
					resFile << endl;
				}
                resFile << iSize / 1024 << "_Kbits\t";
                std::copy(testResults.begin(), testResults.end(), std::ostream_iterator<double>(resFile, "\t"));
				resFile << endl;
			}
			//----------------Extra infos-----------------//
            {
                if (i <= 0) {
                    std::copy(testNames.begin(), testNames.end(), std::ostream_iterator<string>(extraFile, "\t"));
                    resFile << endl;
                }
                resFile << iSize << "_Kbits\t";
                std::copy(pValues.begin(), pValues.end(), std::ostream_iterator<double>(extraFile, "\t"));
                resFile << endl;
            }
		}
		resFile.close();
        extraFile.close();
	}

    cout << endl << "Whole time expend: " << (my_get_current_clock_time() - wholeTimeExpend) / 1000.
         << "sec." << endl;
	

	return 0;
}
