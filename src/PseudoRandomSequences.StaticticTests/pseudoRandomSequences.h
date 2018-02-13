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

    using std::vector;
    using std::pair;
    using std::string;

    class TestParameters {
    public:
        TestParameters(uint64_t EPSILON_SIZE);

        struct BookStackPair { uint64_t upperPart; uint64_t dimension; };
        vector<BookStackPair> bookStackTest;

        vector<uint64_t> blockFrequencyTest;
        vector<uint64_t> nonOverlappingTemplateMatchingsTest;
        vector<uint64_t> overlappingTemplateMatchingsTest;		// TODO: CHECK
        vector<uint64_t> linearComplexityTest;	// must be > 3
        vector<uint64_t> serialTest;
        vector<uint64_t> approximateEntropyTest;
    };

    vector<string> getStatisticTestNames(string testKey, size_t sequenceSize);

	typedef size_t Word;
	const uint32_t MAX_DIMENSION = sizeof(Word) * 8;
	using AlphabetType = std::bitset<MAX_DIMENSION>;

    void runTests(BoolIterator epsilonBegin,
        BoolIterator epsilonEnd,
        std::vector<double> & testResults,
        std::string const & testKey,
        const std::string &uniqueSequenceName);

	int beaconRun(int argc, char * argv[]);

	int generatorsTestConfigRun(int argc, char * argv[]);

}

#endif //PSEUDO_RANDOM_SEQUENCES
