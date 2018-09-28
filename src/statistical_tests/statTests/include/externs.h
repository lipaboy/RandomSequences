#pragma once

#include "defs.h"
#include <vector>
#include <stdio.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                   G L O B A L   D A T A  S T R U C T U R E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace statistical_tests_space {

typedef std::vector<BitSequence> Sequence;
typedef Sequence::iterator BoolIterator;

extern TP			tp;						// TEST PARAMETER STRUCTURE
extern FILE			*freqfp;				// FILE OUTPUT STREAM
extern FILE			*summary;				// FILE OUTPUT STREAM

extern char	generatorDir[NUMOFGENERATORS][20];
extern char	testNames[NUMOFTESTS+1][32];

}

