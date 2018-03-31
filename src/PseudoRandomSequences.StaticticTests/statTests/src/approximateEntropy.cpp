#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../include/externs.h"
#include "../include/utilities.h"
#include "../include/cephes.h"  
#include "../include/stat_fncs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
                A P P R O X I M A T E  E N T R O P Y   T E S T
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace statistical_tests_space {

double
ApproximateEntropy(int m, int n, BoolIterator epsilon)
{
	int				i, j, k, r, blockSize, seqLength, powLen, index;
	double			sum, numOfBlocks, ApEn[2], apen, chi_squared, p_value;
	unsigned int	*P;
	
	//fprintf(stats[TEST_APEN], "\t\t\tAPPROXIMATE ENTROPY TEST\n");
	//fprintf(stats[TEST_APEN], "\t\t--------------------------------------------\n");
	//fprintf(stats[TEST_APEN], "\t\tCOMPUTATIONAL INFORMATION:\n");
	//fprintf(stats[TEST_APEN], "\t\t--------------------------------------------\n");
	//fprintf(stats[TEST_APEN], "\t\t(a) m (block length)    = %d\n", m);

	seqLength = n;
	r = 0;
	
	for ( blockSize=m; blockSize<=m+1; blockSize++ ) {
		if ( blockSize == 0 ) {
			ApEn[0] = 0.00;
			r++;
		}
		else {
			numOfBlocks = (double)seqLength;
			powLen = (int)pow(2, blockSize+1)-1;
			if ( (P = (unsigned int*)calloc(powLen,sizeof(unsigned int)))== NULL ) {
				printf("ApEn:  Insufficient memory available.\n");
				return -1.;
			}
			for ( i=1; i<powLen-1; i++ )
				P[i] = 0;
			for ( i=0; i<numOfBlocks; i++ ) { /* COMPUTE FREQUENCY */
				k = 1;
				for ( j=0; j<blockSize; j++ ) {
					k <<= 1;
					auto iter = epsilon;
					std::advance(iter, (i + j) % seqLength);
					if ( (int)*iter == 1 )
						k++;
				}
				P[k-1]++;
			}
			/* DISPLAY FREQUENCY */
			sum = 0.0;
			index = (int)pow(2, blockSize)-1;
			for ( i=0; i<(int)pow(2, blockSize); i++ ) {
				if ( P[index] > 0 )
					sum += P[index]*log(P[index]/numOfBlocks);
				index++;
			}
			sum /= numOfBlocks;
			ApEn[r] = sum;
			r++;
			free(P);
		}
	}
	apen = ApEn[0] - ApEn[1];
	
	//apen = 0;
	chi_squared = 2.0*seqLength*(log(2) - apen);
	p_value = cephes_igamc(pow(2, m-1), chi_squared/2.0);
	
	//fprintf(stats[TEST_APEN], "\t\t(b) n (sequence length) = %d\n", seqLength);
	//fprintf(stats[TEST_APEN], "\t\t(c) Chi^2               = %f\n", chi_squared);
	//fprintf(stats[TEST_APEN], "\t\t(d) Phi(m)	       = %f\n", ApEn[0]);
	//fprintf(stats[TEST_APEN], "\t\t(e) Phi(m+1)	       = %f\n", ApEn[1]);
	//fprintf(stats[TEST_APEN], "\t\t(f) ApEn                = %f\n", apen);
	//fprintf(stats[TEST_APEN], "\t\t(g) Log(2)              = %f\n", log(2.0));
	//fprintf(stats[TEST_APEN], "\t\t--------------------------------------------\n");

	if ( m > (int)(log(seqLength)/log(2)-5) ) {
		printf("\t\tNote: The blockSize = %d exceeds recommended value of %d\n", m,
			MAX(1, (int)(log(seqLength)/log(2)-5)));
		printf("\t\tResults are inaccurate!\n");
		printf( "\t\t--------------------------------------------\n");
	}
	
	//printf("Approximate Entropy:\t%s\t\tp_value = %f\n\n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value); //fflush(stats[TEST_APEN]);
	return double(p_value);
	/*fprintf(results[TEST_APEN], "%f\n", p_value); fflush(results[TEST_APEN]);*/
}

}
