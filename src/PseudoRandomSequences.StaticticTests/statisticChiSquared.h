#ifndef STATISTIC_CHI_SQUARED
#define STATISTIC_CHI_SQUARED

#include <vector>
#include <cmath>
#include <boost/math/distributions/chi_squared.hpp>

namespace statistical_tests_space {

    using std::vector;
    using size_type = size_t;

    inline double statisticChiSquared(double observedValue, double expectedValue) {
        return std::pow(std::abs(observedValue - expectedValue)
                        //- 0.5			//Yates correction
                    , 2) / expectedValue;
    }

    template <class Iterator>
    double statisticChiSquared(Iterator observedSequenceBegin, Iterator observedSequenceEnd, double expectedValue) {
        double statisticX2 = 0.;
        for (auto it = observedSequenceBegin; it != observedSequenceEnd; it++) {
            statisticX2 += statisticChiSquared(*it, expectedValue);
		}
		return statisticX2;
	}

//    template <typename Iterator>
//    double statisticChiSquaredTest(Iterator begin, Iterator end, size_type dimension) {
//		if (dimension <= 0)		// TODO: throw exception
//            return 0.;

//        size_type alphabetSize = 1 << dimension;
//        vector<size_type> frequences(alphabetSize, 0);
//		AlphabetType symbol;
//        for (auto iter = begin; iter != end; iter++) {
//            symbol[i % dimension] = *iter;
//			// You have real dependency by data
//			if (0 == (i + 1) % dimension) {
//                size_type & currFreq = frequences[static_cast<size_type>(symbol.to_ullong())];
//				// What's better: destroyed conveyor and less operations write
//				// or unbroken conveyor with more write condition operations (not in memory)
//				currFreq += 1;
//			}
//		}

//		double expectedNumber = sequence.size() / (dimension * alphabetSize * 1.);
//        double statisticX2 = statisticChiSquared(frequences.begin(), frequences.end(), expectedNumber);
//		double possibility = 1 -
//			boost::math::cdf(boost::math::chi_squared_distribution<double>(alphabetSize - 1),
//				statisticX2);

//		return possibility;
//	}

}

#endif //STATISTIC_CHI_SQUARED



