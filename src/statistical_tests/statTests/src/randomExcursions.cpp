#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../include/externs.h"
#include "../include/cephes.h"  
#include "../include/stat_fncs.h"

using namespace statistical_tests_space;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                     R A N D O M  E X C U R S I O N S  T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::vector<double>
statistical_tests_space::doRandomExcursionsTest(int n, BoolIterator epsilon)
{
	int		b, i, j, k, J, x;
	int		cycleStart, cycleStop, *cycle = NULL, *S_k = NULL;
	int		stateX[8] = { -4, -3, -2, -1, 1, 2, 3, 4 };
	int		counter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	double	p_value, sum, constraint, nu[6][8];
	double	pi[5][6] = { {0.0000000000, 0.00000000000, 0.00000000000, 0.00000000000, 0.00000000000, 0.0000000000}, 
						 {0.5000000000, 0.25000000000, 0.12500000000, 0.06250000000, 0.03125000000, 0.0312500000},
						 {0.7500000000, 0.06250000000, 0.04687500000, 0.03515625000, 0.02636718750, 0.0791015625},
						 {0.8333333333, 0.02777777778, 0.02314814815, 0.01929012346, 0.01607510288, 0.0803755143},
						 {0.8750000000, 0.01562500000, 0.01367187500, 0.01196289063, 0.01046752930, 0.0732727051} };

	std::vector<double> result;

	//printf("RandomExcursions:");

	if ( ((S_k = (int *)calloc(n, sizeof(int))) == NULL) ||
		 ((cycle = (int *)calloc(MAX(1000, n/100), sizeof(int))) == NULL) ) {
		printf("Random Excursions Test:  Insufficent Work Space Allocated.\n");
		if ( S_k != NULL )
			free(S_k);
		if ( cycle != NULL )
			free(cycle);
		return result;
	}
	
	J = 0; 					/* DETERMINE CYCLES */
	S_k[0] = 2*(int)(*epsilon) - 1;
	for( i=1; i<n; i++ ) {
		S_k[i] = S_k[i-1] + 2 * (*epsilon++) - 1;
		if ( S_k[i] == 0 ) {
			J++;
			if ( J > MAX(1000, n/100) ) {
				printf("ERROR IN FUNCTION randomExcursions:  EXCEEDING THE MAX NUMBER OF CYCLES EXPECTED\n.");
				free(S_k);
				free(cycle);
				return result;
			}
			cycle[J] = i;
		}
	}
	if ( S_k[n-1] != 0 )
		J++;
	cycle[J] = n;

	/*fprintf(stats[TEST_RND_EXCURSION], "\t\t\t  RANDOM EXCURSIONS TEST\n");
	fprintf(stats[TEST_RND_EXCURSION], "\t\t--------------------------------------------\n");
	fprintf(stats[TEST_RND_EXCURSION], "\t\tCOMPUTATIONAL INFORMATION:\n");
	fprintf(stats[TEST_RND_EXCURSION], "\t\t--------------------------------------------\n");
	fprintf(stats[TEST_RND_EXCURSION], "\t\t(a) Number Of Cycles (J) = %04d\n", J);
	fprintf(stats[TEST_RND_EXCURSION], "\t\t(b) Sequence Length (n)  = %d\n", n);*/

	constraint = MAX(0.005*pow(n, 0.5), 500);
	if (J < constraint) {
//        std::cerr << "\t\tRandom Excursions Test:\n";
//        std::cerr << "\t\t---------------------------------------------\n";
//        std::cerr << "\t\tWARNING:  TEST NOT APPLICABLE.  THERE ARE AN\n";
//        std::cerr << "\t\t\t  INSUFFICIENT NUMBER OF CYCLES.\n";
//        std::cerr << "\t\t---------------------------------------------\n";
		/*for(i = 0; i < 8; i++)
			printf( "%f\n", 0.0);*/
		//result = -1.;
	}
	else {
		//fprintf(stats[TEST_RND_EXCURSION], "\t\t(c) Rejection Constraint = %f\n", constraint);
		//fprintf(stats[TEST_RND_EXCURSION], "\t\t-------------------------------------------\n");

		cycleStart = 0;
		cycleStop  = cycle[1];
		for ( k=0; k<6; k++ )
			for ( i=0; i<8; i++ )
				nu[k][i] = 0.;
		for ( j=1; j<=J; j++ ) {                           /* FOR EACH CYCLE */
			for ( i=0; i<8; i++ )
				counter[i] = 0;
			for ( i=cycleStart; i<cycleStop; i++ ) {
				if ( (S_k[i] >= 1 && S_k[i] <= 4) || (S_k[i] >= -4 && S_k[i] <= -1) ) {
					if ( S_k[i] < 0 )
						b = 4;
					else
						b = 3;
					counter[S_k[i]+b]++;
				}
			}
			cycleStart = cycle[j]+1;
			if ( j < J )
				cycleStop = cycle[j+1];
			
			for ( i=0; i<8; i++ ) {
				if ( (counter[i] >= 0) && (counter[i] <= 4) )
					nu[counter[i]][i]++;
				else if ( counter[i] >= 5 )
					nu[5][i]++;
			}
		}
		
		
		for ( i=0; i<8; i++ ) {
			x = stateX[i];
			sum = 0.;
			for ( k=0; k<6; k++ )
				sum += pow(nu[k][i] - J*pi[(int)fabs(x)][k], 2) / (J*pi[(int)fabs(x)][k]);
			p_value = cephes_igamc(2.5, sum/2.0);
			
			if ( isNegative(p_value) || isGreaterThanOne(p_value) )
				printf("WARNING:  P_VALUE IS OUT OF RANGE.\n");

			/*if (p_value >= ALPHA)
				successCount++;*/
			result.push_back(p_value);
			
			//fprintf(results[TEST_RND_EXCURSION], "%f\n", p_value); fflush(results[TEST_RND_EXCURSION]);
		}
		//printf("\t\tsuccess = %d of %d\n", successCount, 8);
		//result = double(successCount) / 8;
	} 
	//fprintf(stats[TEST_RND_EXCURSION], "\n"); fflush(stats[TEST_RND_EXCURSION]);
	free(S_k);
	free(cycle);

	return result;
}
