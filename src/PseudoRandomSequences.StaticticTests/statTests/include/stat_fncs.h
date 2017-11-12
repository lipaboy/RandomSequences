
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


bool	Frequency(int n);
bool	BlockFrequency(int M, int n);
bool	CumulativeSums(int n);
bool	Runs(int n);
bool	LongestRunOfOnes(int n);
bool	Rank(int n);
bool	DiscreteFourierTransform(int n);
// <successes count, templates count>
std::pair<int, int>	NonOverlappingTemplateMatchings(int m, int n);
bool	OverlappingTemplateMatchings(int m, int n);
bool	Universal(int n);
bool	ApproximateEntropy(int m, int n);
std::pair<int, int>	RandomExcursions(int n);
std::pair<int, int>	RandomExcursionsVariant(int n);
bool	LinearComplexity(int M, int n);
bool	Serial(int m, int n);

#endif //STAT_FNCS_H