#ifndef STATISTIC_CHI_SQUARED
#define STATISTIC_CHI_SQUARED

#include <vector>
#include <cmath>

#include <boost/range/any_range.hpp>

namespace PseudoRandomSequences {

	using std::vector;

	using SizeTAnyRange = boost::any_range<
		size_t,
		boost::bidirectional_traversal_tag,
		size_t,
		std::ptrdiff_t
	>;

	double statisticChiSquared(SizeTAnyRange frequences, double expectedValue) {
		double statisticX2 = 0;
		for (auto it = frequences.begin(); it != frequences.end(); it++) {
			statisticX2 +=
				std::pow(std::abs(*it - expectedValue) 
					//- 0.5			//Yates correction
					, 2) / expectedValue;
		}
		return statisticX2;
	}

	double statisticChiSquaredTest(vector<bool> const & sequence, uint32_t dimension) {
		if (dimension <= 0)		// TODO: throw exception
			return 0;

		//for (u)
		return 0;
	}

}

#endif //STATISTIC_CHI_SQUARED



