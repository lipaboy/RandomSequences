#ifndef STAT_TESTS_EXTERNS_H
#define STAT_TESTS_EXTERNS_H

#include "defs.h"
#include <vector>
#include <boost/range/any_range.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                   G L O B A L   D A T A  S T R U C T U R E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//typedef boost::any_range<
//	bool,
//	boost::bidirectional_traversal_tag,
//	bool &,
//	std::ptrdiff_t
//> BoolAnyRange;

typedef std::vector<BitSequence> Sequence;
typedef Sequence::iterator BoolIterator;

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
