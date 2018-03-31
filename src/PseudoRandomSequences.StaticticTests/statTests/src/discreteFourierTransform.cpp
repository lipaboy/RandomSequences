#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
//#include "../include/externs.h"
//#include "../include/utilities.h"
//#include "cephes.h"

#include "../include/defs.h"
#include "../include/assess.h"
#include "../include/stat_fncs.h"

#include <vector>
#include <iostream>

using namespace statistical_tests_space;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         D I S C R E T E  F O U R I E R  T R A N S F O R M  T E S T 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double
statistical_tests_space::DiscreteFourierTransform(int n, BoolIterator epsilon)
{
	double	p_value, upperBound, percentile, N_l, N_o, d, *m = NULL, *X = NULL, *wsave = NULL;
	int		i, count, ifac[15];

	if ( ((X = (double*) calloc(n,sizeof(double))) == NULL) ||
		 ((wsave = (double *)calloc(2*n,sizeof(double))) == NULL) ||
		 ((m = (double*)calloc(n/2+1, sizeof(double))) == NULL) ) {
//			fprintf(stats[7],"\t\tUnable to allocate working arrays for the DFT.\n");
			if( X != NULL )
				free(X);
			if( wsave != NULL )
				free(wsave);
			if( m != NULL )
				free(m);
            printf("DiscreteFourierTrans : failure by malloc/n");
			return -1.;
	}
    for ( i=0; i<n; i++ ) {
        X[i] = 2 * (*(epsilon++)) - 1;
    }
		//X[i] = 2 * getRand(i) - 1;
		//X[i] = 2 * (int)seq[i] - 1;
	
	__ogg_fdrffti(n, wsave, ifac);		/* INITIALIZE WORK ARRAYS */
	__ogg_fdrfftf(n, X, wsave, ifac);	/* APPLY FORWARD FFT */
	
	m[0] = sqrt(X[0]*X[0]);	    /* COMPUTE MAGNITUDE */
	
	for ( i=0; i<n/2; i++ )
		m[i+1] = sqrt(pow(X[2*i+1],2)+pow(X[2*i+2],2)); 
	count = 0;				       /* CONFIDENCE INTERVAL */
	upperBound = sqrt(2.995732274*n);
	for ( i=0; i < n/2; i++ )
		if ( m[i] < upperBound )
			count++;
	percentile = (double)count/(n/2)*100;
	N_l = (double) count;       /* number of peaks less than h = sqrt(3*n) */
	N_o = (double) 0.95*n/2.0;
	d = (N_l - N_o)/sqrt(n/4.0*0.95*0.05);
	p_value = erfc(fabs(d)/sqrt(2.0));

//    printf("\t/t/t/tFFT TEST/n");
//    printf("\t/t-------------------------------------------\n");
//    printf("\t/tCOMPUTATIONAL INFORMATION:\n");
//    printf("\t/t-------------------------------------------\n");
//    printf("\t/t(a) Percentile = %f/n", percentile);
//    printf("\t/t(b) N_l        = %f/n", N_l);
//    printf("\t/t(c) N_o        = %f/n", N_o);
//    printf("\t/t(d) d          = %f/n", d);
//    printf("\t/t-------------------------------------------\n");
/*
    fprintf(stats[TEST_FFT], "%s/t/tp_value = %f/n/n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
    fprintf(results[TEST_FFT], "%f/n", p_value);*/

    //printf( "DiscreteFourier:\t/t%s/t/tp_value = %.15lf/n/n", p_value < ALPHA ? "FAILURE" : "SUCCESS", p_value);
	

	free(X);
	free(wsave);
	free(m);

	return double(p_value);
}
