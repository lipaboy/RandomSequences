#ifndef DISCRETE_FOURIER_TRANSFORM_TEST_H
#define DISCRETE_FOURIER_TRANSFORM_TEST_H

#include <vector>
#include <cmath>
#include <iostream>

#include <boost\range\any_range.hpp>

namespace PseudoRandomSequences {

	typedef boost::any_range<
		bool,
		boost::bidirectional_traversal_tag,
		bool,
		std::ptrdiff_t
	> BoolAnyRange;

	//Slow test O(N ^ 2) or O(N ^ 2 * 3 / 8)
	template <class BoolSequenceRandAccessContainer>
	void discreteFourierTransformTest(const BoolSequenceRandAccessContainer& sequence) {
		//Calculate only from 0 to n/2 - 1 (because symmetry)
		using std::vector;

		const uint32_t size = sequence.size();
		const uint32_t halfSize = size / 2 + (size % 2 == 1);
		//Sums of imaginaries part
		vector<double> imSums(halfSize);
		
		//Converse sequence from {0, 1} to {-1, +1}
		const double PI = std::acos(-1.0);
		for (uint32_t j = 0; j < imSums.size(); //only first half
										j++) {
			imSums[j] = 0;
			for (uint32_t k = j; k < sequence.size(); k++) {
				imSums[j] += (2 * static_cast<double>(sequence[k]) - 1) *
					std::sin(2 * PI * k * j / size);
				if (k < imSums.size())
					imSums[k] += imSums[j];
			}
		}
	}

}

#endif //DESCRETE_FOURIER_TRANSFORM_TEST_H