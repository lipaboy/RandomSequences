#ifndef UTILITIES_STAT_TEST_H
#define UTILITIES_STAT_TEST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
              U T I L I T Y  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "config.h"

namespace statistical_tests_space {

int convertToBits(BYTE *x, int xBitLength, int bitsNeeded, int *num_0s, int *num_1s, int *bitsRead, Sequence& epsilon);

}

#endif //UTILITIES_STAT_TEST_H
