
#ifndef STAT_FNCS_H
#define STAT_FNCS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     S T A T I S T I C A L  T E S T  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "externs.h"
#include <vector>

namespace statistical_tests_space {

// TODO: rename all the test functions to camel case

void  __ogg_fdrffti(int n, double *wsave, int *ifac);
void  __ogg_fdrfftf(int n, double *X, double *wsave, int *ifac);

double bookStackTestMain(int argc, const char* argv[]);

double	testFrequency(int n, BoolIterator epsilon);
double	BlockFrequency(int M, int n, BoolIterator epsilon);
std::pair<double, double> 	CumulativeSums(int n, BoolIterator epsilon);
double	Runs(int n, BoolIterator epsilon);
double	LongestRunOfOnes(int n, BoolIterator epsilon);
double	Rank(int n, BoolIterator epsilon);
double	DiscreteFourierTransform(int n, BoolIterator epsilon);
std::vector<double>	NonOverlappingTemplateMatchings(int m, int n, BoolIterator epsilon);
double	OverlappingTemplateMatchings(int m, int n, BoolIterator epsilon);
double	Universal(int n, BoolIterator epsilon);
double	ApproximateEntropy(int m, int n, BoolIterator epsilon);
std::vector<double>	RandomExcursions(int n, BoolIterator epsilon);
std::vector<double>	RandomExcursionsVariant(int n, BoolIterator epsilon);
double	LinearComplexity(int M, int n, BoolIterator epsilon);
std::pair<double, double>	Serial(int m, int n, BoolIterator epsilon);

}

#endif //STAT_FNCS_H
