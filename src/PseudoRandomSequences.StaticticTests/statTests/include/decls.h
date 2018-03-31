#ifndef DECLS_STAT_TEST_H
#define DECLS_STAT_TEST_H

#include <stdio.h>
#include "../include/defs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                   G L O B A L   D A T A  S T R U C T U R E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
namespace statistical_tests_space {

TP			tp;						// TEST PARAMETER STRUCTURE
FILE		*freqfp;				// FILE OUTPUT STREAM
FILE		*summary;				// FILE OUTPUT STREAM
int			testVector[NUMOFTESTS+1];

char	generatorDir[NUMOFGENERATORS][20] = { "AlgorithmTesting", "LCG", "QCG1", "QCG2","CCG", "XOR",
			"MODEXP", "BBS", "MS", "G-SHA1" };
				
char	testNames[NUMOFTESTS+1][32] = { " ", "Frequency", "BlockFrequency", "CumulativeSums", "Runs", "LongestRun", "Rank",
			"FFT", "NonOverlappingTemplate", "OverlappingTemplate", "Universal", "ApproximateEntropy", "RandomExcursions",
			"RandomExcursionsVariant", "Serial", "LinearComplexity" };

}

#endif //DECLS_STAT_TEST_H
