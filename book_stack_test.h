#ifndef BOOK_STACK_TEST_H
#define BOOK_STACK_TEST_H

#include <vector>

namespace PseudoRandomSequences {

	//TODO: rewrite on any_range (because this func only reading and only in series)

	template <typename AlphabetType, class SequenceRandAccessContainer>
	double bookStackTest(const SequenceRandAccessContainer& seq, uint32_t alphabetSize) {
		using std::vector;

		//map: alphabet -> stack position
		vector<uint32_t> stack(alphabetSize);
		//map: { stack position } -> frequency of meeting symbol onto this position
		//moving this symbol to stack peek
		vector<uint32_t> freq(alphabetSize);

		for (uint32_t i = 0; i < stack.size(); i++) {
			stack[i] = i;
			freq[i] = 0;
		}

		for (uint32_t t = 1; t <= seq.size(); t++) {
			const uint32_t currSymbol = seq[t - 1];
			const uint32_t prevPos = stack[currSymbol];

			//old stack state (t - 1)
			//if (prevPos == currSymbol)	--wrong
			freq[prevPos]++;

			if (prevPos > 0)		//not necessary
				for (uint32_t i = 0; i < stack.size(); i++)
					if (stack[i] < prevPos)
						++stack[i];				//move down
			stack[currSymbol] = 0;		//move to stack peek (up)

										//new stack state (t)
		}
		//Meaning: symbols with equal possibility can be (turn out to be) on any stack position
		//TODO: replace (N / 2) on (N / alphabetSize)
		const uint32_t N = seq.size();

		/*double statisticX2 = 0;
		for (uint32_t j = 0; j < freq.size(); j++) {
		statisticX2 += std::pow(freq[j] - (N / 2.0), 2) / (N / 2.0);
		}*/

		double statisticX2Correction = 0;	//Yates's correction
		double expectedNumber = N / (alphabetSize * 1.0);
		for (uint32_t j = 0; j < freq.size(); j++) {
			statisticX2Correction += std::pow(std::abs(freq[j] - expectedNumber) - 0.5, 2) / expectedNumber;
		}
		//std::cout << "StatisticX^2 = " << statisticX2 << "\t Yates = " << statisticX2Correction << endl;

		return statisticX2Correction;
	}

}

#endif //BOOK_STACK_TEST_H
