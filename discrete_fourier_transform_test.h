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

	//Slow test O(N ^ 2) 
	//or O(N ^ 2 * 3 / 8 + N / 2) --old
	template <class BoolSequenceRandAccessContainer>
	double discreteFourierTransformTest(const BoolSequenceRandAccessContainer& sequence) {
		//Calculate only from 0 to n/2 - 1 (because symmetry)
		using std::vector;
		using std::complex;
		using namespace std::complex_literals;

		const uint32_t size = sequence.size();
		const uint32_t halfSize = //size / 2 + (size % 2 == 1);
			size;
		//Complex sums
		vector<complex<double> > sums(halfSize);
		
		//Converse sequence from {0, 1} to {-1, +1}
		const double PI = std::acos(-1.0);
		for (size_t j = 0; j < sums.size(); j++) {
			sums[j] = 0. + 0i;
			//wrong
			const double seqj = static_cast<double>(sequence[j]);

			for (size_t k = 0; // j;	// (1)
					k < sequence.size(); k++) 
			{
				const double & seqk = static_cast<double>(sequence[k]);
				complex<double> val = 
					std::exp((2 * PI * 1i) * static_cast<double>(k * j) / static_cast<double>(size));
				sums[j] += (2 * seqk - 1) * val;
				// What be faster : (1) write to variable that isn't into cache or (2)calculate complex exponent value again??
				// (2) with 3e3 seq length faster than (1) (4100ms vs 5600ms)

				// (1)
				/*if (k < sums.size())
					sums[k] += (2 * seqj - 1) * val;*/
			}
		}

		bool flag = true;
		double sum1(0), sum2(0);
		for (size_t i = 0; i < sums.size() / 2; i++) {
			double temp = std::abs(sums[i]);
			if (std::abs(temp - 1000) < 20)
				std::cout << "ind: " << i << std::endl;
			sum1 += temp;
		}
		for (size_t i = sums.size() / 2; i < sums.size(); i++) {
			double temp = std::abs(sums[i]);
			if (std::abs(temp - 1000) < 20)
				std::cout << "ind: " << i << std::endl;
			sum2 += temp;
		}
		std::cout << sum1 << " " << sum2 << std::endl;

		const double confidenceIntervalBorder = std::sqrt(std::log(1. / 0.05) * size);
		const double expectedPeaks = 0.95 * halfSize;
			//size / 2.;		// Maybe you don't need to divide on 2
										// because you don't calculate half sum
		uint32_t observedPeaks = 0;
		for (size_t i = 0; i < sums.size(); i++)
			if (std::abs(sums[i]) < confidenceIntervalBorder)
				observedPeaks++;
		const double distance = (observedPeaks - expectedPeaks) / std::sqrt(size * 0.95 * 0.05 / 4.);
		const double pValue = std::erfc(std::abs(distance) / std::sqrt(2));

		//at the 1% level (if pValue > 0.01 then sequence is random)
		return pValue 
			- 0.01
			;
	}

}

#endif //DESCRETE_FOURIER_TRANSFORM_TEST_H