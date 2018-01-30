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

    struct ResultStructure;

	void runTests(BoolIterator epsilonBegin,
		BoolIterator epsilonEnd,
		std::vector<std::string> & testNames,
		bool isSaveNames,
		std::vector<double> & testResults,
        std::string const & testKey
        //std::string const & inputFile
                  );

    void testGenerator(std::string generatorName);

	int beaconRun(int argc, char * argv[]);

	int generatorsTestConfigRun(int argc, char * argv[]);


}

#endif //PSEUDO_RANDOM_SEQUENCES
