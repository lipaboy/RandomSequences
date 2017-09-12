#ifndef BOOK_STACK_TEST_H
#define BOOK_STACK_TEST_H

#include <vector>
#include <bitset>
#include <random>
#include <algorithm>
#include <boost/math/distributions/chi_squared.hpp>

#include "statisticChiSquared.h"

namespace PseudoRandomSequences {

	/*------------------
		The size_t type is the unsigned integer type that is the result of the 
		sizeof operator (and the offsetof operator), so it is guaranteed to be big enough 
		to contain the size of the biggest object your system can handle(e.g., a static array of 8Gb).

		The size_t type may be bigger than, equal to, or smaller than an unsigned int, 
		and your compiler might make assumptions about it for optimization.
	------------------*/
	typedef size_t Word;
	const uint32_t MAX_DIMENSION = sizeof(Word) * 8;

	//TODO: rewrite on any_range (because this func only reading and only in series)

	//Complexity: O(N * |Alphabet|)
	//return possibility of Sequence Randomness
	//template <class BoolSequenceRandAccessContainer
	//	//= std::vector<bool> 
	//>
	double bookStackTest(const std::vector<bool>& seq, 
			uint32_t dimension) //degree
	{
		using std::vector;
		using AlphabetType = std::bitset<MAX_DIMENSION>;

		if (dimension <= 0)	// TODO: throw exception
			return 0;

		Word alphabetSize = 1 << dimension;
		//map: alphabet -> stack position
		vector<Word> stack(alphabetSize);
		//map: { stack position } -> frequency of meeting symbol onto this position
		//moving this symbol to stack peek
		vector<Word> freq(alphabetSize, 0);

		for (Word i = 0; i < stack.size(); i++)
			stack[i] = i;

		Word scaledSize = seq.size() - seq.size() % dimension;
		for (Word t = 0; t < scaledSize; t += dimension)
		{
			AlphabetType currSymbol;
			for (uint32_t i = 0; i < dimension; i++)
				currSymbol[i] = seq[i + t];

			const Word prevPos = stack[static_cast<Word>(currSymbol.to_ullong())];
			freq[prevPos]++;

			if (prevPos > 0)	//if alphabetSize is small you can add inside of cycle
				for (Word i = 0; i < stack.size(); i++)
					stack[i] = (stack[i] < prevPos) ? 1 + stack[i] : stack[i];	//move down
			stack[static_cast<Word>(currSymbol.to_ullong())] = 0;		//move to stack peek (up)
		}

		//------------Calculate Chi-squared--------------//

		//Meaning: symbols with equal possibility can be (turn out to be) on any stack position
		const Word N = seq.size() / dimension;
		double expectedNumber = N / (alphabetSize * 1.0);
		double statisticX2Yates = statisticChiSquared(freq, expectedNumber);	//Yates's correction
		
		//std::cout <<  "\t Yates = " << statisticX2Correction << std::endl;

		double possibility = 1 - 
			boost::math::cdf(boost::math::chi_squared_distribution<double>(alphabetSize - 1),
				statisticX2Yates);

		return possibility;
	}

}

#endif //BOOK_STACK_TEST_H
