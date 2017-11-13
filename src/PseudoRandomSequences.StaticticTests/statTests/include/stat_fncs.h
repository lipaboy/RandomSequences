
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

double	Frequency(int n);
double	BlockFrequency(int M, int n);
std::pair<double, double> 	CumulativeSums(int n);
double	Runs(int n);
double	LongestRunOfOnes(int n);
double	Rank(int n);
double	DiscreteFourierTransform(int n);
std::vector<double>	NonOverlappingTemplateMatchings(int m, int n);
double	OverlappingTemplateMatchings(int m, int n);
double	Universal(int n);
double	ApproximateEntropy(int m, int n);
std::vector<double>	RandomExcursions(int n);
std::vector<double>	RandomExcursionsVariant(int n);
double	LinearComplexity(int M, int n);
std::pair<double, double>	Serial(int m, int n);

#endif //STAT_FNCS_H