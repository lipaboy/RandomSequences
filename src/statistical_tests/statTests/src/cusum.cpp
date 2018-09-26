#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../include/externs.h"
#include "../include/cephes.h"
#include "../include/stat_fncs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		    C U M U L A T I V E  S U M S  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace statistical_tests_space;

std::pair<double, double>
statistical_tests_space::doCumulativeSums(int n, BoolIterator epsilon)
{
	int		S, sup, inf, z, zrev, k;
	double	sum1, sum2, p_value;

	S = 0;
	sup = 0;
	inf = 0;
	for ( k=0; k<n; k++ ) {
		*(epsilon++) ? S++ : S--;
		if ( S > sup )
			sup++;
		if ( S < inf )
			inf--;
		z = (sup > -inf) ? sup : -inf;
		zrev = (sup-S > S-inf) ? sup-S : S-inf;
	}
	
	// forward
	sum1 = 0.0;
	for ( k=(-n/z+1)/4; k<=(n/z-1)/4; k++ ) {
		sum1 += cephes_normal(((4*k+1)*z)/sqrt(n));
		sum1 -= cephes_normal(((4*k-1)*z)/sqrt(n));
	}
	sum2 = 0.0;
	for ( k=(-n/z-3)/4; k<=(n/z-1)/4; k++ ) {
		sum2 += cephes_normal(((4*k+3)*z)/sqrt(n));
		sum2 -= cephes_normal(((4*k+1)*z)/sqrt(n));
	}

	p_value = 1.0 - sum1 + sum2;
	
	/*fprintf(stats[TEST_CUSUM], "\t\t      CUMULATIVE SUMS (FORWARD) TEST\n");
	fprintf(stats[TEST_CUSUM], "\t\t-------------------------------------------\n");
	fprintf(stats[TEST_CUSUM], "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stats[TEST_CUSUM], "\t\t-------------------------------------------\n");
	fprintf(stats[TEST_CUSUM], "\t\t(a) The maximum partial sum = %d\n", z);
	fprintf(stats[TEST_CUSUM], "\t\t-------------------------------------------\n");*/

	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
		printf("\t\tWARNING:  P_VALUE IS OUT OF RANGE\n");

	//printf("%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
	double p_value1 = p_value;
	//fprintf(results[TEST_CUSUM], "%f\n", p_value);
		
	// backwards
	sum1 = 0.0;
	for ( k=(-n/zrev+1)/4; k<=(n/zrev-1)/4; k++ ) {
		sum1 += cephes_normal(((4*k+1)*zrev)/sqrt(n));
		sum1 -= cephes_normal(((4*k-1)*zrev)/sqrt(n));
	}
	sum2 = 0.0;
	for ( k=(-n/zrev-3)/4; k<=(n/zrev-1)/4; k++ ) {
		sum2 += cephes_normal(((4*k+3)*zrev)/sqrt(n));
		sum2 -= cephes_normal(((4*k+1)*zrev)/sqrt(n));
	}
	p_value = 1.0 - sum1 + sum2;

	//fprintf(stats[TEST_CUSUM], "\t\t      CUMULATIVE SUMS (REVERSE) TEST\n");
	//fprintf(stats[TEST_CUSUM], "\t\t-------------------------------------------\n");
	//fprintf(stats[TEST_CUSUM], "\t\tCOMPUTATIONAL INFORMATION:\n");
	//fprintf(stats[TEST_CUSUM], "\t\t-------------------------------------------\n");
	//fprintf(stats[TEST_CUSUM], "\t\t(a) The maximum partial sum = %d\n", zrev);
	//fprintf(stats[TEST_CUSUM], "\t\t-------------------------------------------\n");

	if ( isNegative(p_value) || isGreaterThanOne(p_value) )
		printf("\t\tWARNING:  P_VALUE IS OUT OF RANGE\n");

	//printf("Cumulative Sums:\t\t%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);// fflush(stats[TEST_CUSUM]);
	return std::make_pair(p_value1, p_value);
	//fprintf(results[TEST_CUSUM], "%f\n", p_value); fflush(results[TEST_CUSUM]);
}
