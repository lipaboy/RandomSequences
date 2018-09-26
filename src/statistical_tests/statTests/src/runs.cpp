#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../include/externs.h"
#include "../include/cephes.h"
#include "../include/stat_fncs.h"

using namespace statistical_tests_space;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                              R U N S  T E S T 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double
statistical_tests_space::doRunsTest(int n, BoolIterator epsilon)
{
	int		S, k;
	double	pi, V, erfc_arg, p_value;

	S = 0;
	BoolIterator epsilonTemp = epsilon;
	for ( k=0; k<n; k++ )
		if (*(epsilonTemp++))
			S++;
	pi = (double)S / (double)n;

	if ( fabs(pi - 0.5) > (2.0 / sqrt(n)) ) {
		printf("\t\t\t\tRUNS TEST\n");
		printf("\t\t------------------------------------------\n");
		printf("\t\tPI ESTIMATOR CRITERIA NOT MET! PI = %f\n", pi);
		return -1.;
	}
	else {

		V = 1;
		for (k = 1; k < n; k++) {
			auto prev = *epsilon;
			if (prev != *(++epsilon))
				V++;
		}
	
		erfc_arg = fabs(V - 2.0 * n * pi * (1-pi)) / (2.0 * pi * (1-pi) * sqrt(2*n));
		p_value = erfc(erfc_arg);
		
		//fprintf(stats[TEST_RUNS], "\t\t\t\tRUNS TEST\n");
		//fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		//fprintf(stats[TEST_RUNS], "\t\tCOMPUTATIONAL INFORMATION:\n");
		//fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		//fprintf(stats[TEST_RUNS], "\t\t(a) Pi                        = %f\n", pi);
		//fprintf(stats[TEST_RUNS], "\t\t(b) V_n_obs (Total # of runs) = %d\n", (int)V);
		//fprintf(stats[TEST_RUNS], "\t\t(c) V_n_obs - 2 n pi (1-pi)\n");
		//fprintf(stats[TEST_RUNS], "\t\t    -----------------------   = %f\n", erfc_arg);
		//fprintf(stats[TEST_RUNS], "\t\t      2 sqrt(2n) pi (1-pi)\n");
		//fprintf(stats[TEST_RUNS], "\t\t------------------------------------------\n");
		if ( isNegative(p_value) || isGreaterThanOne(p_value) )
			printf("WARNING:  P_VALUE IS OUT OF RANGE.\n");

		//printf("Runs (depends on Frequency):\t%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);// fflush(stats[TEST_RUNS]);
		
	}
	return double(p_value);
	//fprintf(results[TEST_RUNS], "%f\n", p_value); fflush(results[TEST_RUNS]);
}
