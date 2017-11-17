
#ifndef STAT_FNCS_H
#define STAT_FNCS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     S T A T I S T I C A L  T E S T  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "externs.h"
#include <vector>


//#define _CRT_SECURE_NO_WARNINGS

// TODO: move all the tests into namespace

void  __ogg_fdrffti(int n, double *wsave, int *ifac);
void  __ogg_fdrfftf(int n, double *X, double *wsave, int *ifac);

double bookStackTestMain(int argc, const char* argv[]);

double	Frequency(int n, BoolAnyRange epsilon);
double	BlockFrequency(int M, int n, BoolAnyRange epsilon);
std::pair<double, double> 	CumulativeSums(int n, BoolAnyRange epsilon);
double	Runs(int n, BoolAnyRange epsilon);
double	LongestRunOfOnes(int n, BoolAnyRange epsilon);
double	Rank(int n, BoolAnyRange epsilon);
double	DiscreteFourierTransform(int n, BoolAnyRange epsilon);
std::vector<double>	NonOverlappingTemplateMatchings(int m, int n, BoolAnyRange epsilon);
double	OverlappingTemplateMatchings(int m, int n, BoolAnyRange epsilon);
double	Universal(int n, BoolAnyRange epsilon);
double	ApproximateEntropy(int m, int n, BoolAnyRange epsilon);
std::vector<double>	RandomExcursions(int n, BoolAnyRange epsilon);
std::vector<double>	RandomExcursionsVariant(int n, BoolAnyRange epsilon);
double	LinearComplexity(int M, int n, BoolAnyRange epsilon);
std::pair<double, double>	Serial(int m, int n, BoolAnyRange epsilon);

#endif //STAT_FNCS_H