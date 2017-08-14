#ifndef DISCRETE_FOURIER_TRANSFORM_TEST_H
#define DISCRETE_FOURIER_TRANSFORM_TEST_H

#include <vector>
#include <cmath>
#include <iostream>
#include <complex>
#include <algorithm>

#include <boost\range\any_range.hpp>

namespace PseudoRandomSequences {

	typedef boost::any_range<
		bool,
		boost::bidirectional_traversal_tag,
		bool,
		std::ptrdiff_t
	> BoolAnyRange;

	//Slow test O(N ^ 2) or O(N ^ 2 * 3 / 8 + N / 2)
	template <class BoolSequenceRandAccessContainer>
	bool discreteFourierTransformTest(const BoolSequenceRandAccessContainer& sequence) {
		//Calculate only from 0 to n/2 - 1 (because symmetry)
		using std::vector;
		using std::complex;
		using namespace std::complex_literals;

		const uint32_t size = sequence.size();
		const uint32_t halfSize = size / 2 + (size % 2 == 1);
			//size;
		//Complex sums
		vector<complex<double> > sums(halfSize);
		complex<double> com(5.0, 1.0);
		
		//Converse sequence from {0, 1} to {-1, +1}
		const double PI = std::acos(-1.0);
		for (uint32_t j = 0; j < sums.size(); //only first half
										j++) {
			sums[j] = 0. + 0i;
			const double seqj = static_cast<double>(sequence[j]);
			for (uint32_t k = j; k < sequence.size(); k++) {
				const double & seqk = static_cast<double>(sequence[k]);
				complex<double> val = 
					std::exp((2 * PI * 1i) * static_cast<double>(k * j) / static_cast<double>(size));
				sums[j] += (2 * seqk - 1) * val;
				if (k < sums.size())
					sums[k] += (2 * seqj - 1) * val;
			}
		}

		const double confidenceIntervalBorder = std::sqrt(std::log(1. / .05) * size);
		const double expectedPeaks = .95 * size / 2.;
		uint32_t observedPeaks = 0;
		for (uint32_t i = 0; i < sums.size(); i++)
			if (std::abs(sums[i]) < confidenceIntervalBorder)
				observedPeaks++;
		const double distance = (observedPeaks - expectedPeaks) / std::sqrt(size * .95 * .05 / 4.);
		const double pValue = std::erfc(std::abs(distance) / std::sqrt(2));

		//at the 1% level
		return (pValue >= 0.01);
	}

}

#endif //DESCRETE_FOURIER_TRANSFORM_TEST_H