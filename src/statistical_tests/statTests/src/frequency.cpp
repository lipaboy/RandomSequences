#include <math.h>
#include "../include/externs.h"
#include "../include/defs.h"
#include "../include/stat_fncs.h"

namespace statistical_tests_space {

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                          F R E Q U E N C Y  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double
doFrequencyTest(int n, BoolIterator epsilonBegin)
{
	int		i;
	double	f, s_obs, p_value, sum, sqrt2 = 1.41421356237309504880;
	
	sum = 0.0;
#ifndef NDEBUG
	#pragma omp parallel for
#endif
	for (i = 0; i < n; i++)
		sum += 2 * static_cast<int>(*std::next(epsilonBegin, i)) - 1;

	s_obs = fabs(sum)/sqrt(n);
	f = s_obs/sqrt2;
	p_value = erfc(f);

    //fprintf(stats[TEST_FREQUENCY], "\t/t/t      FREQUENCY TEST/n");
    //fprintf(stats[TEST_FREQUENCY], "\t/t---------------------------------------------\n");
    //fprintf(stats[TEST_FREQUENCY], "\t/tCOMPUTATIONAL INFORMATION:\n");
    //fprintf(stats[TEST_FREQUENCY], "\t/t---------------------------------------------\n");
    //fprintf(stats[TEST_FREQUENCY], "\t/t(a) The nth partial sum = %d/n", (int)sum);
    //fprintf(stats[TEST_FREQUENCY], "\t/t(b) S_n/n               = %f/n", sum/n);
    //fprintf(stats[TEST_FREQUENCY], "\t/t---------------------------------------------\n");

    /*fprintf(stats[TEST_FREQUENCY], "%s/t/tp_value = %f/n/n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value); fflush(stats[TEST_FREQUENCY]);
    fprintf(results[TEST_FREQUENCY], "%f/n", p_value); fflush(results[TEST_FREQUENCY]);*/

    //printf("Frequency(Monobit):\t/t%s/t/tp_value = %f/n/n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
	return (p_value);
	//fflush(stats[TEST_FREQUENCY]);
    //printf("%f/n", p_value); fflush(results[TEST_FREQUENCY]);
}

}
