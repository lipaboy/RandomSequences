
#ifndef STAT_FNCS_H
#define STAT_FNCS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     S T A T I S T I C A L  T E S T  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "externs.h"
#include <vector>

namespace statistical_tests_space {

void  __ogg_fdrffti(int n, double *wsave, int *ifac);
void  __ogg_fdrfftf(int n, double *X, double *wsave, int *ifac);

double doBookStackTest(int argc, const char* argv[]);

double	doFrequencyTest(int n, BoolIterator epsilon);
double	doBlockFrequencyTest(int M, int n, BoolIterator epsilon);
std::pair<double, double> 	doCumulativeSums(int n, BoolIterator epsilon);
double	doRunsTest(int n, BoolIterator epsilon);
double	doLongestRunOfOnesTest(int n, BoolIterator epsilon);
double	doRankTest(int n, BoolIterator epsilon);
double	doDiscreteFourierTransformTest(int n, BoolIterator epsilon);
std::vector<double>	doNonOverlappingTemplateMatchingsTest(int m, int n, BoolIterator epsilon);
double	doOverlappingTemplateMatchingsTest(int m, int n, BoolIterator epsilon);
double	doUniversalTest(int n, BoolIterator epsilon);
double	doApproximateEntropyTest(int m, int n, BoolIterator epsilon);
std::vector<double>	doRandomExcursionsTest(int n, BoolIterator epsilon);
std::vector<double>	doRandomExcursionsVariantTest(int n, BoolIterator epsilon);
double	doLinearComplexityTest(int M, int n, BoolIterator epsilon);
std::pair<double, double>	doSerialTest(int m, int n, BoolIterator epsilon);

}

#endif //STAT_FNCS_H
