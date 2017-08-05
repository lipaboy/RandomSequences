#include "../lipaboyLibrary/src/intervals/interval.h"
#include "../lipaboyLibrary/src/maths/accuracy_number.h"
#include "sequence_converter.h"
//#include "isequence.h"

#include <iostream>
#include <iterator>
#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace PseudoRandomSequences;
using namespace LipaboyMaths;

//#include <boost\iterator\transform_iterator.hpp>
//#include <boost\bind.hpp>
//#include <boost\log\utility\functional\bind.hpp>
//#include <boost\range\any_range.hpp>

//TODO: try to use GoogleTests

int main(void) {
	time_t t;
	srand((unsigned)time(&t));

	/*--------------Crypto----------------*/

	string filename = "BinaryMatrixGenerator.txt";

	uint32_t k = 4;
	SequenceConverter converter(k);

	for (; k <= 12; k++) {
		converter.setDimension(k);
		Sequence seq(1e3);		//TODO: test on sequence size and Dimension (2D test)

		//possibility(1) = 1 / 10
		std::generate(seq.begin(), seq.end(),
			[]() -> AlphabetType { return ((0 == std::rand() % 10) ? 1 : 0); }
		);
		Sequence newSeq = converter.converse(seq);
		//cout << newSeq << endl;
		std::cout << "Dim = " << k << std::endl;
		bookStackTest(newSeq);
	}

	cout << "My test:" << endl;
	//My test sequence
	Sequence gap(40);		//~loophole
	std::generate(gap.begin(), gap.end(), 
		[]() -> AlphabetType { static int i = 0; return (((i++) % 4 < 2) ? 1 : 0); }
	);
	std::copy(gap.cbegin(), gap.cend(), std::ostream_iterator<AlphabetType>(cout, " "));
	std::cout << std::endl;
	bookStackTest(gap);


	//boost::any_range anyRange;
	
	/*--------------Boost Library-----------------*/
	//int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	//const int N = sizeof(x) / sizeof(int);

	//
	////typedef boost::bind< std::multiplies<int>, int, int > Function;
	//typedef binder1st< std::multiplies<int> > Function;
	//typedef boost::transform_iterator<Function, int*> doubling_iterator;

	//doubling_iterator i(x, bind1st(std::multiplies<int>(), 2)),
	//	i_end(x + N, bind1st(std::multiplies<int>(), 2));

	//std::cout << "multiplying the array by 2:" << std::endl;
	//while (i != i_end)
	//	std::cout << *(i++) << " ";
	//std::cout << std::endl;

	//std::cout << "adding 4 to each element in the array:" << std::endl;
	//std::copy(boost::make_transform_iterator(x, bind1st(std::plus<int>(), 4)),
	//	boost::make_transform_iterator(x + N, bind1st(std::plus<int>(), 4)),
	//	std::ostream_iterator<int>(std::cout, " "));
	//std::cout << std::endl;

	/*--------------AccuracyNumber----------------*/

	/*PositiveRay<int, std::less<> > ray(5);
	if (ray.contains(INT_MAX))
		std::cout << "ray (5, +inf) contains int_max" << std::endl;
	AccuracyDouble d1(5.0, 1.0);
	if (d1 < 6.1)
		std::cout << "5 < 6.1 with eps = 1" << std::endl;
	if (d1 >= 6.0)
		std::cout << "5 >= 6 with eps = 1" << std::endl;
	if (d1 >= AccuracyDouble(5.5, 1.0))
		std::cout << "5 >= 5.5 with eps = 1" << std::endl;*/


		/*--------------ConstAccuracyNumber----------------*/
	/*ConstAccuracyNumber<double, int, 1, -2> cd1(5.0);
	constexpr double dPow = std::exp(5.0);*/


	/*--------------Intervals----------------*/

	/*OpenInterval<int> openInterval(0, 5);
	if (openInterval.contains(4)) {
		cout << "4 in (0, 5)" << endl;
	}
	CloseInterval<int> closeInterval(0, 5);
	if (closeInterval.contains(5)) {
		cout << "5 in [0, 5]" << endl;
	}*/
	//PositiveInfinity<int> plusInfinity;
	////std::comp
	//if (plusInfinity > 100)
	//	cout << "+inf > 100" << endl;
	//if (100 < plusInfinity)
	//	cout << "100 < +inf" << endl;

	/*Interval<int, std::less<>, std::less<> > interval(0, 5);

	if (interval.in(1))
		std::cout << "1 lays into interval" << std::endl;
	if (interval.out(0))
		std::cout << "0 out of interval" << std::endl;
	if (interval.containsAll(1, 2, 3, 4))
		std::cout << "1, 2, 3, 4 lay into interval" << std::endl;
	if (interval.containsAny(0, 1, 5))
		std::cout << "0, 1 or 5 lay into interval" << std::endl;*/

	/*IPlenty<int> *plenty = new Interval<int>(-1, 4);
	if (plenty -> in(cutOffBorder(-4, 4, 0))) {
		cout << "3 in (0, 4)" << endl;
	}
	delete plenty;*/

	

	return 0;
}

