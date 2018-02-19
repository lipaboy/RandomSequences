#ifndef UTILITIES_STAT_TEST_H
#define UTILITIES_STAT_TEST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
              U T I L I T Y  F U N C T I O N  P R O T O T Y P E S 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "config.h"

//int		displayGeneratorOptions();
//int		generatorOptions(char** streamFile);
//void	chooseTests();
//void	fixParameters();
//void	fileBasedBitStreams(char *streamFile);
//void	readBinaryDigitsInASCIIFormat(FILE *fp, char *streamFile);
//void	readHexDigitsInBinaryFormat(FILE *fp);
int		convertToBits(BYTE *x, int xBitLength, int bitsNeeded, int *num_0s, int *num_1s, int *bitsRead, Sequence& epsilon);
//void	openOutputStreams(int option);
//void	invokeTestSuite(int option, char *streamFile);
//void	nist_test_suite();

#endif //UTILITIES_STAT_TEST_H
