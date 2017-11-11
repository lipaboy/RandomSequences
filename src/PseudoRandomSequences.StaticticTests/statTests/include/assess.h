#ifndef ASSESS_H
#define ASSESS_H

#include <stdlib.h>
#include <random>

inline double getRand(int i) {
	//srand(time(NULL));
	return rand() % 2;
	//static auto chi = std::chi_squared_distribution<double>(5.0);
	//return chi.operator()<double>(5.0);

}

//int
//main_assess(int argc, char *argv[], double d);

#endif //#ifndef ASSESS_H