#include "book_stack_test.h"
#include "discrete_fourier_transform_test.h"
#include "sequence_converter.h"

//#include "isequence.h"

#include <iostream>
#include <iterator>
#include <time.h>
#include <stdlib.h>
#include <string>

//using namespace std;
using namespace PseudoRandomSequences;
//using namespace LipaboyMaths;

//#include <boost\iterator\transform_iterator.hpp>
//#include <boost\bind.hpp>
//#include <boost\log\utility\functional\bind.hpp>
//#include <boost\range\any_range.hpp>

//TODO: try to use GoogleTests

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/filtered.hpp>

template <class Type>
using AnyRange = boost::any_range<
	Type,
	boost::bidirectional_traversal_tag,
	Type,
	std::ptrdiff_t
>;

#include <iomanip>
#include <limits>
#include <cmath>

typedef std::vector<bool> Sequence;
typedef MatrixRandomConverter<Sequence > VectorMatrixRandomGenerator;

int main(void) {
	time_t t;
	srand((unsigned)time(&t));

	/*--------------Crypto----------------*/

	//std::vector<int> vec({ 0, 1, 2 });
	//UIntAnyRange anyRange = vec;
	/*std::array<bool> vec({ 0, 1, 0 });
	AnyRange<bool> range = vec;
	boost::copy(range, std::ostream_iterator<bool>(std::cout, " "));*/
	
		//Useful funcs
	//boost::copy(anyRange, std::ostream_iterator<int>(std::cout, " "));
	//double d[] = { 1,2,3,4 };
	//std::vector<double> x(d, d + 4);
	//std::vector<double> y;
	//boost::copy(x | boost::adaptors::filtered([](double xx) -> bool { return xx > 2.0; }), 
	//	std::back_inserter(y));
	//boost::copy(y, std::ostream_iterator<double>(std::cout, " "));
	using std::string;
	using std::vector;
	string filename = "BinaryMatrixGenerator.txt";

	//for all the converters the same sequence
	//3.141592653589793239
	//3,1415926535897932384626433832795
	//3.14159265358979323116

	//double pi = std::acos(static_cast<double>(-1.0));
	//std::cout << std::setprecision(21) << " " << pi << std::endl;
	//long double pi2 = std::acos(static_cast<long double>(-1.0));
	//std::cout << std::setprecision(21) << " " << pi2 << std::endl;

	std::vector<VectorMatrixRandomGenerator> converters;
	std::vector<double> statisticBooks;
	std::vector<bool> statisticFourier;
	for (uint32_t i = 4; i <= 4; i += 4) {
		converters.push_back(VectorMatrixRandomGenerator(i));
		statisticBooks.push_back(0.);
		statisticFourier.push_back(false);
	}
	//converters.push_back(SequenceConverter(filename));
	//statisticBooks.push_back(0.0);

	Sequence seq(3e3);
	uint32_t testSize = 1;
	for (uint32_t i = 0; i < testSize; i++) { //--wrong if testSize > 1
		std::generate(seq.begin(), seq.end(),
			[]() -> bool { return ((0 == std::rand() % 10) ? 1 : 0); }
		);
		Sequence result(seq.size());
		for (uint32_t j = 0; j < converters.size(); j++) {
			converters[j].converse(result, seq);
			//statisticBooks[j] += bookStackTest<bool>(result, 2);	--wrong
			statisticFourier[j] = discreteFourierTransformTest(result);
		}
	}
	std::cout << "BookStackTest\tDFTT" << std::endl;
	for (uint32_t i = 0; i < statisticBooks.size(); i++) {
		std::cout << converters[i].getDimension() << ": " << statisticBooks[i]
			<< "\t" << statisticFourier[i] << std::endl;
	}
	/*std::transform(statisticBooks.begin(), statisticBooks.end(), std::ostream_iterator<string>(std::cout),
		[&testSize, &converters](double elem) -> string {
		static int i = 0; 
		return std::to_string(converters[i++].getDimension()) + std::string(": ") 
			+ std::to_string(elem / testSize) + string("\n");
	});*/

	//cout << "My test:" << endl;
	////My test sequence
	//Sequence gap(40);		//~loophole
	//std::generate(gap.begin(), gap.end(), 
	//	[]() -> AlphabetType { static int i = 0; return (((i++) % 4 < 2) ? 1 : 0); }
	//);
	//std::copy(gap.cbegin(), gap.cend(), std::ostream_iterator<AlphabetType>(cout, " "));
	//std::cout << std::endl;
	//bookStackTest(gap);

	/*--------------End Crypto----------------*/


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


	

	return 0;
}

