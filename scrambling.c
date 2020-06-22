//gcc -c scrambling.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "scrambling.h"

#define getbit(byte,nbit)   ((int) (byte >> nbit ) & 1)

uint8_t* scrambler(uint8_t* scramblerInput, int nBytes, int nCellId, uint8_t* scramblerOutput) {
	printf("=============================Scambling PBCH=================================\n");

	uint8_t *x1, *x2, *c;
	int i, j;
	int Nc = 1600;
	x1 = (uint8_t *) malloc(sizeof(uint8_t) * ((nBytes * 8 + Nc) / 8));
	x2 = (uint8_t *) malloc(sizeof(uint8_t) * ((nBytes * 8 + Nc) / 8));// max of 32 bits are stored remaining are 0
	c =  (uint8_t *) malloc(sizeof(uint8_t) * nBytes);

	for ( i = 0 ; i < nBytes ; i++) {
		scramblerOutput[i] = 0;
	}

	for ( i = 0; i < ((nBytes * 8 + 1600) / 8); ++i) {
		x1[i] = 0;
	}

	for ( i = 0; i < ((nBytes * 8 + 1600) / 8); ++i) {
		x2[i] = 0;
	}
	for ( i = 0; i < nBytes ; ++i) {
		c[i] = 0;
	}
//to initialize 31 bits of x1 as     0000000 00000000 0000000 00000001 - 31 bits
	x1[0] = 1;
	x1[1] = 0;
	x1[2] = 0;
	x1[3] = 0;

	for ( i = 31 ; i < (nBytes * 8 + Nc) ; ++i) {
		x1[i / 8] =   x1[i / 8] | ( getbit(x1[(i - 31 + 3) / 8], (i - 31 + 3) % 8) ^ ( getbit(x1[ (i - 31) / 8], (i - 31) % 8)) ) << i % 8  ;
	}

	for ( i = 0; i < 32; ++i)
	{
		x2[i / 8] = x2[i / 8] | (getbit(nCellId, i) << i % 8);
	}

	for ( i = 31; i < (nBytes * 8 + Nc); ++i ) {
		x2[i / 8] = x2[i / 8] | ( getbit(x2[(i - 31 + 3) / 8], (i - 31 + 3) % 8) ^ ( getbit(x2[ (i - 31 + 2) / 8], (i - 31 + 2) % 8)) ^ ( getbit(x2[ (i - 31 + 1) / 8], (i - 31 + 1) % 8)) ^ ( getbit(x2[ (i - 31) / 8], (i - 31 ) % 8)) ) << i % 8  ;
	}

	for (i = 0 ; i < nBytes * 8 ; i++) {
		c[i / 8] = c[i / 8] | (getbit(x1[(i + Nc) / 8], (i + Nc) % 8) ^ getbit(x2[(i + Nc) / 8], (i + Nc) % 8)) << i % 8;
	}



	for (i = 0 ; i < nBytes * 8 ; i++) {
		scramblerOutput[i / 8] = scramblerOutput[i / 8] | ( getbit(scramblerInput[i / 8] , i % 8) ^ getbit(c[i / 8], i % 8 )) << i % 8;
	}

	// for ( i = 0; i < nBytes*8; ++i)
	// {
	// 	printf("%x", getbit(scramblerInput[i/8],i%8));
	// }
	// printf("\n");

	for ( i = 0; i < nBytes; ++i)
	{
		printf("%x ", scramblerOutput[i ]);
	}
	printf("\n");
	free(x1);
	free(x2);
	free(c);
	printf("=============================Scambling PBCH=================================\n");
	return scramblerOutput;

}