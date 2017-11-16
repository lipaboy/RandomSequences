#ifndef STAT_TESTS_EXTERNS_H
#define STAT_TESTS_EXTERNS_H

#include "../include/defs.h"
#include <vector>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                   G L O B A L   D A T A  S T R U C T U R E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//extern std::vector<bool>	epsilon;				// BIT STREAM
extern TP			tp;						// TEST PARAMETER STRUCTURE
//extern FILE			*stats[NUMOFTESTS+1];	// FILE OUTPUT STREAM
//extern FILE			*results[NUMOFTESTS+1];	// FILE OUTPUT STREAM
extern FILE			*freqfp;				// FILE OUTPUT STREAM
extern FILE			*summary;				// FILE OUTPUT STREAM
//extern int			testVector[NUMOFTESTS+1];

extern char	generatorDir[NUMOFGENERATORS][20];
extern char	testNames[NUMOFTESTS+1][32];

#endif //STAT_TESTS_EXTERNS_H
