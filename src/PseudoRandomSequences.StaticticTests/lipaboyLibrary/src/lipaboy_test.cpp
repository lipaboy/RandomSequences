#include "lipaboy_test.h"

#include <iostream>
#include <array>
#include <time.h>

typedef unsigned long long int ullint;
typedef long long int llint;

#include <windows.h>

namespace LipaboyLib {

	void bitContainerTests();

	int lipaboyMain() {
		using std::cout;
		using std::endl;

		/*--------------AccuracyNumber----------------*/

	/*	PositiveRay<int, std::less<> > ray(5);
		if (ray.contains(INT_MAX))
		std::cout << "ray (5, +inf) contains int_max" << std::endl;*/

		double a = 1.0;
		PrecisionDouble b(5.0, 1.0);
		cout << b + a << endl;
		b.setNumber(b + a);
		b = 5.0;
		//b.operator=()
		cout << (double)b << endl;
		cout << b - a << endl;
		cout << b - 2.0 * a << endl;
		cout << b / (a * 2.0) << endl;
		cout << b << endl;

		//AccuracyDouble d1(5.0, 1.0);
		//if (d1 < 6.1)
		//std::cout << "5 < 6.1 with eps = 1" << std::endl;
		//if (d1 >= 6.0)
		//std::cout << "5 >= 6 with eps = 1" << std::endl;
		//if (d1 >= AccuracyDouble(5.5, 1.0))
		//std::cout << "5 >= 5.5 with eps = 1" << std::endl;


		/*--------------AccuracyFixedNumber----------------*/
		
		FixedPrecisionNumber<double, int, 1, -1> cd1(5.0);
		if (5.09 >= cd1)
			cout << "5.0 <= 5.09 with 0.1 precision" << endl;
		if (cd1 != 4.89)
			cout << "5.0 != 4.89 with 0.1 precision" << endl;

		cd1 = 5.2 + cd1;
		cout << cd1 << endl;
		//DOESN'T WORK
		Algebra<double> & g = cd1;
		cout << g << endl;

		constexpr int p = powDozen<int>(5);
		cout << p << endl;

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

		/*Containable<int> *plenty = new Interval<int>(-1, 4);
		if (plenty -> in(cutOffBorder(-4, 4, 0))) {
		cout << "3 in (0, 4)" << endl;
		}
		delete plenty;*/

		return 0;
	}

	void bitContainerTests() {
		//TODO: Want I want to do: compare perfomance of vector<int>[i] and bitcon[i] (or maybe do it later?)

		constexpr int d = sizeof(int) * 8;	//It proves that sizeof(int) - compile-time function
		BitVector<> bitcon;
		std::vector<uint32_t> vect;

		#define testSize static_cast<size_t>(2e3)
		clock_t start,
			end;
		//uint32_t arr[testSize] = { 0 };
		std::array<uint32_t, testSize> arr = { 0 };

		//Test 1 : push

		start = clock();
		for (int i = 0; i < testSize; i++)
			vect.push_back(rand() % 2);
		end = clock();
		std::cout << "vect time push: " << end - start << std::endl;

		start = clock();
		for (int i = 0; i < testSize; i++)
			bitcon.pushBack(rand() % 2);
		end = clock();
		std::cout << "bitcon time push: " << end - start << std::endl;

		//Test 2 : read
		int sum;

		start = clock();
		sum = 0;
		for (uint32_t i = 0; i < bitcon.size(); i++)
			sum += bitcon.get(i);
		end = clock();
		std::cout << "bitcon time read: " << end - start << std::endl;

		start = clock();
		sum = 0;
		for (uint32_t i = 0; i < vect.size(); i++) 
			sum += vect[i];
		end = clock();
		std::cout << "vect time read: " << end - start << std::endl;

		start = clock();
		sum = 0;
		for (uint32_t i = 0; i < vect.size(); i++) 
			sum += arr[i];
		end = clock();
		std::cout << "arr time read: " << end - start << std::endl;

		//Test 3 : write

		start = clock();
		for (uint32_t i = 0; i < bitcon.size(); i++)
		bitcon.set(i, rand() % 2);
		end = clock();
		std::cout << "bitcon time write: " << end - start << std::endl;

		start = clock();
		for (uint32_t i = 0; i < vect.size(); i++)
			vect[i] = rand() % 2;
		end = clock();
		std::cout << "vect time write: " << end - start << std::endl;

		start = clock();
		for (uint32_t i = 0; i < vect.size(); i++)
			arr[i] = rand() % 2;
		end = clock();
		std::cout << "arr time write: " << end - start << std::endl;
		
		
	}

}