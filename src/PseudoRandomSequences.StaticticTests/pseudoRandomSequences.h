#ifndef PSEUDO_RANDOM_SEQUENCES
#define PSEUDO_RANDOM_SEQUENCES

#include <vector>
#include <bitset>
#include <boost/math/distributions/chi_squared.hpp>

namespace PseudoRandomSequences {

	typedef size_t Word;
	const uint32_t MAX_DIMENSION = sizeof(Word) * 8;
	using AlphabetType = std::bitset<MAX_DIMENSION>;

	void runTests(std::vector<bool> const & epsilon,
		std::vector<std::string> & testNames,
		std::vector<double> & testResults,
		std::string const & testKey,
		int fileIndex,
		std::string const & inputFile);

	int beaconRun(int argc, char * argv[]);

	int generatorsTestConfigRun(int argc, char * argv[]);

}

#endif //PSEUDO_RANDOM_SEQUENCES
