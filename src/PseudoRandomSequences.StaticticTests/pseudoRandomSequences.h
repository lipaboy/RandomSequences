#ifndef PSEUDO_RANDOM_SEQUENCES
#define PSEUDO_RANDOM_SEQUENCES

#include <vector>
#include <bitset>
#include <boost/math/distributions/chi_squared.hpp>

namespace PseudoRandomSequences {

	typedef size_t Word;
	const uint32_t MAX_DIMENSION = sizeof(Word) * 8;
	using AlphabetType = std::bitset<MAX_DIMENSION>;

}

#endif //PSEUDO_RANDOM_SEQUENCES
