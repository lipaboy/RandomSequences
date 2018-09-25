#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "../include/externs.h"
#include "../include/utilities.h"
#include "../include/generators.h"
#include "../include/genutils.h"

#include "../../pseudoRandomSequences.h"

namespace statistical_tests_space {

double
lcg_rand(int N, double SEED, double* DUNIF, int NDIM)
{
	int    i;
	double	DZ, DOVER, DZ1, DZ2, DOVER1, DOVER2;
	double	DTWO31, DMDLS, DA1, DA2;

	DTWO31 = 2147483648.0; /* DTWO31=2**31  */
	DMDLS  = 2147483647.0; /* DMDLS=2**31-1 */
	DA1 = 41160.0;       /* DA1=950706376 MOD 2**16 */
	DA2 = 950665216.0;   /* DA2=950706376-DA1 */

	DZ = SEED;
	if ( N > NDIM )
		N = NDIM;
	for ( i=1; i<=N; i++ ) {
		DZ = floor(DZ);
		DZ1 = DZ*DA1;
		DZ2 = DZ*DA2;
		DOVER1 = floor(DZ1/DTWO31);
		DOVER2 = floor(DZ2/DTWO31);
		DZ1 = DZ1-DOVER1*DTWO31;
		DZ2 = DZ2-DOVER2*DTWO31;
		DZ = DZ1+DZ2+DOVER1+DOVER2;
		DOVER = floor(DZ/DMDLS);
		DZ = DZ-DOVER*DMDLS;
		DUNIF[i-1] = DZ/DMDLS;
		SEED = DZ;
	}

	return SEED;
}

Sequence
lcg()
{
	double	*DUNIF, SEED;
	int		i, counter;
	unsigned bit;
	int		num_0s, num_1s, v, bitsRead;

    Sequence epsilon;

	SEED = 23482349.0;
	epsilon.resize(tp.n);
	if ( ((DUNIF = (double*)calloc(tp.n, sizeof(double))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}
	counter = 1;
 
	for ( v=0; v<tp.numOfBitStreams; v++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		SEED = lcg_rand(static_cast<int>(tp.n), SEED, DUNIF, static_cast<int>(tp.n));
		for ( i=0; static_cast<size_t>(i) < tp.n; i++ ) {
			if ( DUNIF[i] < 0.5 ) {
				bit = 0;
				num_0s++;
			}
			else {
				bit = 1;
				num_1s++;
			}
			bitsRead++;
			epsilon[i] = bit;
		}
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	free(DUNIF);
	//free(epsilon);
	return std::move(epsilon);
}


Sequence
quadRes1()
{
	int		k, num_0s, num_1s, bitsRead, done;
	BYTE	p[64], g[64], x[128];
    Sequence epsilon;
	epsilon.resize(tp.n);

    ahtopb((char*)"987b6a6bf2c56a97291c445409920032499f9ee7ad128301b5d0254aa1a9633fdbd378d40149f1e23a13849f3d45992f5c4c6b7104099bc301f6005f9d8115e1", p, 64);
    ahtopb((char*)"3844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);
	num_0s = 0;
	num_1s = 0;
	done = 0;
	bitsRead = 0;
	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		done = 0;
		bitsRead = 0;
		do {
			memset(x, 0x00, 128);
			ModMult(x, g, 64, g, 64, p,64);
			memcpy(g, x+64, 64);
			done = convertToBits(g, 512, static_cast<int>(tp.n), &num_0s, &num_1s, &bitsRead, epsilon);
		} while ( !done );
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	//free(epsilon);

	return std::move(epsilon);
}

Sequence
quadRes2()
{
	BYTE	g[64], x[129], t1[65];
	BYTE	One[1], Two, Three[1];
	int		k, num_0s, num_1s, bitsRead, done;
    Sequence epsilon;
	epsilon.resize(tp.n);
	/*if ( ((epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/
	One[0] = 0x01;
	Two = 0x02;
	Three[0] = 0x03;

    ahtopb((char*)"7844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);
	
	for( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		done = 0;
		bitsRead = 0;
		do {
			memset(t1, 0x00, 65);
			memset(x, 0x00, 129);
			smult(t1, Two, g, 64);		/* 2x */
			add(t1, 65, Three, 1);		/* 2x+3 */
			Mult(x, t1, 65, g, 64);		/* x(2x+3) */
			add(x, 129, One, 1);		/* x(2x+3)+1 */
			memcpy(g, x+65, 64);
			done = convertToBits(g, 512, static_cast<int>(tp.n), &num_0s, &num_1s, &bitsRead, epsilon);
		} while ( !done) ;
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	//free(epsilon);

	return std::move(epsilon);
}

Sequence
cubicRes()
{
	BYTE	g[64], tmp[128], x[192];
	int		k, num_0s, num_1s, bitsRead, done;
    Sequence epsilon;
	epsilon.resize(tp.n);
	/*if ( ((epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/
	
    ahtopb((char*)"7844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		done = 0;
		do {
			memset(tmp, 0x00, 128);
			memset(x, 0x00, 192);
			Mult(tmp, g, 64, g, 64);
			Mult(x, tmp, 128, g, 64); // Don't need to mod by 2^512, just take low 64 bytes
			memcpy(g, x+128, 64);
			done = convertToBits(g, 512, static_cast<int>(tp.n), &num_0s, &num_1s, &bitsRead, epsilon);
		} while ( !done );
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	//free(epsilon);

	return std::move(epsilon);
}

Sequence
exclusiveOR()
{
	int		i, num_0s, num_1s, bitsRead;
	BYTE	bit_sequence[127];
	
	/*if ( ((epsilon = (BitSequence *)calloc(tp.n,sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/
    Sequence epsilon;
	epsilon.resize(tp.n);

	memcpy(bit_sequence, "0001011011011001000101111001001010011011101101000100000010101111111010100100001010110110000000000100110000101110011111111100111", 127);
	num_0s = 0;
	num_1s = 0;
	bitsRead = 0;
	for (i=0; i<127; i++ ) {
		if ( bit_sequence[i]  ) {
			epsilon[bitsRead] = 1;
			num_1s++;
		}
		else {
			epsilon[bitsRead] = 0;
			num_1s++;
		}
		bitsRead++;
	}
	for ( i=127; static_cast<size_t>(i) < tp.n * tp.numOfBitStreams; i++ ) {
		if ( bit_sequence[(i-1)%127] != bit_sequence[(i-127)%127] ) {
			bit_sequence[i%127] = 1;
			epsilon[bitsRead] = 1;
			num_1s++;
		}
		else {
			bit_sequence[i%127] = 0;
			epsilon[bitsRead] = 0;
			num_0s++;
		}
		bitsRead++;
		if ( bitsRead == tp.n ) {
			//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
			//nist_test_suite();
			num_0s = 0;
			num_1s = 0;
			bitsRead = 0;
		}
	}
	//free(epsilon);
		
	return std::move(epsilon);
}


Sequence
modExp()
{
	int		k, num_0s, num_1s, bitsRead, done;
	BYTE	p[64], g[64], x[192], y[20];
    Sequence epsilon;
    epsilon.resize(tp.n * tp.numOfBitStreams);
	/*if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/
    ahtopb((char*)"7AB36982CE1ADF832019CDFEB2393CABDF0214EC", y, 20);
    ahtopb((char*)"987b6a6bf2c56a97291c445409920032499f9ee7ad128301b5d0254aa1a9633fdbd378d40149f1e23a13849f3d45992f5c4c6b7104099bc301f6005f9d8115e1", p, 64);
    ahtopb((char*)"3844506a9456c564b8b8538e0cc15aff46c95e69600f084f0657c2401b3c244734b62ea9bb95be4923b9b7e84eeaf1a224894ef0328d44bc3eb3e983644da3f5", g, 64);

	for ( k=0; k<tp.numOfBitStreams; k++ ) {
		num_0s = 0;
		num_1s = 0;
        bitsRead = k * tp.n;
		done = 0;
		do {
			memset(x, 0x00, 128);
			ModExp(x, g, 64, y, 20, p, 64);	      /* NOTE:  g must be less than p */
            done = convertToBits(x, 512, static_cast<int>(tp.n) + k * tp.n, &num_0s, &num_1s, &bitsRead, epsilon);
			memcpy(y, x+44, 20);
        } while ( !done );
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	//free(epsilon);

	return std::move(epsilon);
}

Sequence
bbs()
{
    using namespace statistical_tests_space;

	int		i, v, bitsRead;
	BYTE	p[64], q[64], n[128], s[64], x[256];
	int		num_0s, num_1s;
    Sequence epsilon;

    epsilon.resize(tp.n * tp.numOfBitStreams);
	/*if ( (epsilon = (BitSequence*)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/
    ahtopb((char*)"E65097BAEC92E70478CAF4ED0ED94E1C94B154466BFB9EC9BE37B2B0FF8526C222B76E0E915017535AE8B9207250257D0A0C87C0DACEF78E17D1EF9DC44FD91F", p, 64);
    ahtopb((char*)"E029AEFCF8EA2C29D99CB53DD5FA9BC1D0176F5DF8D9110FD16EE21F32E37BA86FF42F00531AD5B8A43073182CC2E15F5C86E8DA059E346777C9A985F7D8A867", q, 64);
	memset(n, 0x00, 128);
	Mult(n, p, 64, q, 64);
	memset(s, 0x00, 64);
    ahtopb((char*)"10d6333cfac8e30e808d2192f7c0439480da79db9bbca1667d73be9a677ed31311f3b830937763837cb7b1b1dc75f14eea417f84d9625628750de99e7ef1e976", s, 64);
	memset(x, 0x00, 256);
	ModSqr(x, s, 64, n, 128);

	for ( v=0; v<tp.numOfBitStreams; v++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;

        for ( i=0; static_cast<size_t>(i) < tp.n; i++ ) {
            ModSqr(x, x, 128, n, 128);
			memcpy(x, x+128, 128);
			if ( (x[127] & 0x01) == 0 ) {
				num_0s++;
                epsilon[i + v * tp.n] = 0;
			}
			else {
				num_1s++;
                epsilon[i + v * tp.n] = 1;
			}
            bitsRead++;
			//if ( (i % 50000) == 0 )
				//printf("\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s);
        }
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
    }
	return std::move(epsilon);
}


// The exponent, e, is set to 11
// This results in k = 837 and r = 187
Sequence
micali_schnorr()
{
	long	i, j;
	int		k=837, num_0s, num_1s, bitsRead, done;
	BYTE	p[64], q[64], n[128], e[1], X[128], Y[384], Tail[105];
    Sequence epsilon;
	epsilon.resize(tp.n);
	/*if ( (epsilon = (BitSequence *)calloc(tp.n, sizeof(BitSequence))) == NULL ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/
    ahtopb((char*)"E65097BAEC92E70478CAF4ED0ED94E1C94B154466BFB9EC9BE37B2B0FF8526C222B76E0E915017535AE8B9207250257D0A0C87C0DACEF78E17D1EF9DC44FD91F", p, 64);
    ahtopb((char*)"E029AEFCF8EA2C29D99CB53DD5FA9BC1D0176F5DF8D9110FD16EE21F32E37BA86FF42F00531AD5B8A43073182CC2E15F5C86E8DA059E346777C9A985F7D8A867", q, 64);
	memset(n, 0x00, 128);
	Mult(n, p, 64, q, 64);
	e[0] = 0x0b;
	memset(X, 0x00, 128);
    ahtopb((char*)"237c5f791c2cfe47bfb16d2d54a0d60665b20904ec822a6", X+104, 24);

	for ( i=0; i<tp.numOfBitStreams; i++ ) {
		num_0s = 0;
		num_1s = 0;
		bitsRead = 0;
		do {
			ModExp(Y, X, 128, e, 1, n, 128);
			memcpy(Tail, Y+23, 105);
			for ( j=0; j<3; j++ )
				bshl(Tail, 105);
			done = convertToBits(Tail, k, static_cast<int>(tp.n), &num_0s, &num_1s, &bitsRead, epsilon);
			memset(X, 0x00, 128);
			memcpy(X+104, Y, 24);
			for ( j=0; j<5; j++ )
				bshr(X+104, 24);
		} while ( !done );

		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	//free(epsilon);
	return std::move(epsilon);
}

//  Uses 160 bit Xkey and no XSeed (b=160)
//  This is the generic form of the generator found on the last page of the Change Notice for FIPS 186-2
Sequence
SHA1()
{
	ULONG	A, B, C, D, E, temp, Wbuff[16];
	BYTE	Xkey[20], G[20], M[64];
	BYTE	One[1] = { 0x01 };
    int		i, num_0s, num_1s, bitsRead;
	int		done;
	ULONG	tx[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
    Sequence epsilon;
    epsilon.resize(tp.n * tp.numOfBitStreams);
	/*if ( ((epsilon = (BitSequence *) calloc(tp.n,sizeof(BitSequence))) == NULL) ) {
		printf("Insufficient memory available.\n");
		exit(1);
	}*/

    ahtopb((char*)"ec822a619d6ed5d9492218a7a4c5b15d57c61601", Xkey, 20);
//	ahtopb("E65097BAEC92E70478CAF4ED0ED94E1C94B15446", Xkey, 20);
//	ahtopb("6BFB9EC9BE37B2B0FF8526C222B76E0E91501753", Xkey, 20);
//	ahtopb("5AE8B9207250257D0A0C87C0DACEF78E17D1EF9D", Xkey, 20);
//	ahtopb("D99CB53DD5FA9BC1D0176F5DF8D9110FD16EE21F", Xkey, 20);
	
	for ( i=0; i<tp.numOfBitStreams; i++ ) {
		num_0s = 0;
		num_1s = 0;
        bitsRead = i * tp.n;
		do {
			memcpy(M, Xkey, 20);
			memset(M+20, 0x00, 44);
			
			// Start: SHA Steps A-E
			A = tx[0];
			B = tx[1];
			C = tx[2];
			D = tx[3];
			E = tx[4];

			memcpy((BYTE *)Wbuff, M, 64);
#ifdef LITTLE_ENDIAN
			byteReverse(Wbuff, 20);
#endif
			sub1Round1( 0 );  sub1Round1( 1 );  sub1Round1( 2 );  sub1Round1( 3 );
			sub1Round1( 4 );  sub1Round1( 5 );  sub1Round1( 6 );  sub1Round1( 7 );
			sub1Round1( 8 );  sub1Round1( 9 );  sub1Round1( 10 ); sub1Round1( 11 );
			sub1Round1( 12 ); sub1Round1( 13 ); sub1Round1( 14 ); sub1Round1( 15 );
			sub2Round1( 16 ); sub2Round1( 17 ); sub2Round1( 18 ); sub2Round1( 19 );
			Round2( 20 ); Round2( 21 ); Round2( 22 ); Round2( 23 );
			Round2( 24 ); Round2( 25 ); Round2( 26 ); Round2( 27 );
			Round2( 28 ); Round2( 29 ); Round2( 30 ); Round2( 31 );
			Round2( 32 ); Round2( 33 ); Round2( 34 ); Round2( 35 );
			Round2( 36 ); Round2( 37 ); Round2( 38 ); Round2( 39 );
			Round3( 40 ); Round3( 41 ); Round3( 42 ); Round3( 43 );
			Round3( 44 ); Round3( 45 ); Round3( 46 ); Round3( 47 );
			Round3( 48 ); Round3( 49 ); Round3( 50 ); Round3( 51 );
			Round3( 52 ); Round3( 53 ); Round3( 54 ); Round3( 55 );
			Round3( 56 ); Round3( 57 ); Round3( 58 ); Round3( 59 );
			Round4( 60 ); Round4( 61 ); Round4( 62 ); Round4( 63 );
			Round4( 64 ); Round4( 65 ); Round4( 66 ); Round4( 67 );
			Round4( 68 ); Round4( 69 ); Round4( 70 ); Round4( 71 );
			Round4( 72 ); Round4( 73 ); Round4( 74 ); Round4( 75 );
			Round4( 76 ); Round4( 77 ); Round4( 78 ); Round4( 79 );
			
			A += tx[0];
			B += tx[1];
			C += tx[2];
			D += tx[3];
			E += tx[4];
			
			memcpy(G, (BYTE *)&A, 4);
			memcpy(G+4, (BYTE *)&B, 4);
			memcpy(G+8, (BYTE *)&C, 4);
			memcpy(G+12, (BYTE *)&D, 4);
			memcpy(G+16, (BYTE *)&E, 4);
#ifdef LITTLE_ENDIAN
			byteReverse((ULONG *)G, 20);
#endif
			// End: SHA Steps A-E

            done = convertToBits(G, 160, static_cast<int>(tp.n) + i * tp.n, &num_0s, &num_1s,
                                 &bitsRead, epsilon);
			add(Xkey, 20, G, 20);
			add(Xkey, 20, One, 1);
		} while ( !done );
		//fprintf(freqfp, "\t\tBITSREAD = %d 0s = %d 1s = %d\n", bitsRead, num_0s, num_1s); fflush(freqfp);
		//nist_test_suite();
	}
	//free(epsilon);
	return std::move(epsilon);
}

}
