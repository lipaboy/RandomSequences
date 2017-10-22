
#ifndef STAT_FNCS_H
#define STAT_FNCS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     S T A T I S T I C A L  T E S T  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "externs.h"
#include <vector>

#define _CRT_SECURE_NO_WARNINGS

void  __ogg_fdrffti(int n, double *wsave, int *ifac);
void  __ogg_fdrfftf(int n, double *X, double *wsave, int *ifac);

double bookStackTestMain(int argc, const char* argv[]);


void	Frequency(int n);
void	BlockFrequency(int M, int n);
//void	CumulativeSums(int n);
void	Runs(int n);
void	LongestRunOfOnes(int n);
void	Rank(int n);
void	DiscreteFourierTransform(int n);
void	NonOverlappingTemplateMatchings(int m, int n);
//void	OverlappingTemplateMatchings(int m, int n);
//void	Universal(int n);
//void	ApproximateEntropy(int m, int n);
//void	RandomExcursions(int n);
//void	RandomExcursionsVariant(int n);
//void	LinearComplexity(int M, int n);
//void	Serial(int m, int n);

#endif //STAT_FNCS_H