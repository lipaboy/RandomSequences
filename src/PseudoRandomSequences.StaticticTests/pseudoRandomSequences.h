#ifndef PSEUDO_RANDOM_SEQUENCES
#define PSEUDO_RANDOM_SEQUENCES

#include <vector>
#include <bitset>
#include <boost/math/distributions/chi_squared.hpp>

#ifdef __GNUC__
#include <unistd.h>
#include <sys/times.h>
#else
#include <time.h>
#endif

#include "statTests/include/stat_fncs.h"

namespace PseudoRandomSequences {

// return in milliseconds
    int my_get_current_clock_time();

	typedef size_t Word;
	const uint32_t MAX_DIMENSION = sizeof(Word) * 8;
	using AlphabetType = std::bitset<MAX_DIMENSION>;

    using std::vector;
    using std::string;

    struct ResultStructure {
        vector<string> statTestNames;
        vector<vector<double> > resultsForEachSize;   //count of failures result of test
        vector<vector<double> > pValues;              //extra data
    };

	void runTests(BoolIterator epsilonBegin,
		BoolIterator epsilonEnd,
        vector<string> & testNames,
		bool isSaveNames,
        vector<double> & testResults,
        string const & testKey
        //std::string const & inputFile
                  );

    ResultStructure testGenerator(string generatorName,
                       vector<int> const & sequenceSizes,
                       string const & testKey,
                       int fieldModulusOfPossibility);

	int beaconRun(int argc, char * argv[]);

	int generatorsTestConfigRun(int argc, char * argv[]);


}

#endif //PSEUDO_RANDOM_SEQUENCES
