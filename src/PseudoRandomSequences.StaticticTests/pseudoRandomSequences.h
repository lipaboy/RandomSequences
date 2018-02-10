#ifndef PSEUDO_RANDOM_SEQUENCES
#define PSEUDO_RANDOM_SEQUENCES

#include <vector>
#include <bitset>
#include <boost/math/distributions/chi_squared.hpp>
#include <chrono>

#ifdef __linux__
#include <unistd.h>
#include <sys/times.h>
#elif _WIN32
#include <time.h>
#endif

#include "statTests/include/stat_fncs.h"

namespace PseudoRandomSequences {

#ifdef __linux__
    using std::chrono::time_point;
    typedef time_point<std::chrono::steady_clock> TimeType;
#elif _WIN32   //Windows
    typedef int TimeType;
#endif

    // return in milliseconds
    TimeType my_get_current_clock_time();

    int getTimeDifferenceInMillis(TimeType const & from, TimeType const & to);

	typedef size_t Word;
	const uint32_t MAX_DIMENSION = sizeof(Word) * 8;
	using AlphabetType = std::bitset<MAX_DIMENSION>;

	void runTests(BoolIterator epsilonBegin,
		BoolIterator epsilonEnd,
		std::vector<std::string> & testNames,
		bool isSaveNames,
		std::vector<double> & testResults,
        std::string const & testKey
        //std::string const & inputFile
                  );

	int beaconRun(int argc, char * argv[]);

	int generatorsTestConfigRun(int argc, char * argv[]);

}

#endif //PSEUDO_RANDOM_SEQUENCES
