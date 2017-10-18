#ifndef STATISTIC_CHI_SQUARED
#define STATISTIC_CHI_SQUARED

#include <vector>
#include <cmath>

#include <boost/range/any_range.hpp>

#include "pseudoRandomSequences.h"

namespace PseudoRandomSequences {

	using std::vector;

	using WordAnyRange = boost::any_range<
		Word,
		boost::bidirectional_traversal_tag,
		Word,
		std::ptrdiff_t
	>;

	double statisticChiSquared(WordAnyRange frequences, double expectedValue) {
		double statisticX2 = 0;
		for (auto it = frequences.begin(); it != frequences.end(); it++) {
			statisticX2 +=
				std::pow(
						std::abs(*it - expectedValue) 
							//- 0.5			//Yates correction
						, 2) / expectedValue;
		}
		return statisticX2;
	}

	template <typename Sequence>
	double statisticChiSquaredTest(Sequence const & sequence, uint32_t dimension) {
		if (dimension <= 0)		// TODO: throw exception
			return 0;

		Word alphabetSize = 1 << dimension;
		vector<Word> frequences(alphabetSize, 0);
		AlphabetType symbol;
		for (Word i = 0; i < sequence.size(); i++) {
			symbol[i % dimension] = sequence[i];
			// You have real dependency by data
			if (0 == (i + 1) % dimension) {
				Word & currFreq = frequences[static_cast<Word>(symbol.to_ullong())];
				// What's better: destroyed conveyor and less operations write 
				// or unbroken conveyor with more write condition operations (not in memory)
				currFreq += 1;
			}
		}

		double expectedNumber = sequence.size() / (dimension * alphabetSize * 1.);
		double statisticX2 = statisticChiSquared(frequences, expectedNumber);
		double possibility = 1 -
			boost::math::cdf(boost::math::chi_squared_distribution<double>(alphabetSize - 1),
				statisticX2);

		return possibility;
	}

}

#endif //STATISTIC_CHI_SQUARED



