//gcc -lm -o pbch pbch.c crc.o convolutionRateMatching.o tailBitConvolution.o scrambling.o
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include<complex.h>
#include "crc.h"
#include "tailBitConvolution.h"
#include "convolutionRateMatching.h"
#include "scrambling.h"
#include "modulation.h"
#include "layermapping.h"


#define getbit(byte,nbit)   ((int) (byte >> nbit ) & 1)

int main() {

	int nBytes = 4;
	uint8_t message[7];
	uint8_t *output, *input;
	int i = 0, E = 1920;
	int print = 1;
	int nCellId = 404;
	int modulation_scheme = 0;
	float complex *cmplx_output, *cmplx_input;
	int n_complex_array_length;

	message[0] = 0xB8;
	message[1] = 0x1F;
	message[2] = 0x2D;
	message[3] = 0x1C;
	//crc
	//message[4] = 0x27;
	// message[5] = 0x46;
	// message[6] = 0x38;

	// TRB block Pring
	if (print == 1) {
		printf("trb from pbch start\n");
		for (i = 0; i < nBytes; ++i)
		{
			printf("%x", message[i] );
		}
		printf("\ntrb from pbch end\n");
	}

	//CRC
	output = crcSlow(message, nBytes, 1, output);
	nBytes = nBytes + 3;
	// CRC OUT PRINT
	if (print == 1) {
		printf("crc output from pbch start\n");
		for (i = 0; i < nBytes; ++i)
		{
			printf("%x", output[i] );
		}
		printf("\ncrc output from pbch end\n");
	}


	//Tailbit convultion encoder
	output = tailBitEncoder(output, nBytes, output);
	//Encoder OUT PRINT
	if (print == 1) {
		printf("taibit output from pbch start\n");
		for (i = 0; i < nBytes * 3; ++i)
		{
			if (i % nBytes == 0   ) {
				printf("\n");
			}
			printf("%x", output[i] );
		}
		printf("\ntaibit output from pbch end\n");
	}


	//convolution rate matching of tail bit encoder
	input = (uint8_t*) malloc(nBytes * 3 * sizeof(uint8_t));
	for (i = 0; i < nBytes * 3; i++) {
		input[i] = output[i];
	}
	output = convolutionRateMatcher(input , nBytes , output );
	// //Ratematching OUT PRINT
	if (print == 1) {
		printf("ratematching output from pbch start\n");
		for (i = 0; i < 1920 / 8; ++i)
		{

			printf("%x ", output[i] );
		}
		printf("\nratematching output from pbch end\n");
	}


	//scrambler
	nBytes = E / 8;
	input = (uint8_t*) realloc(input, nBytes  * sizeof(uint8_t));
	for (i = 0; i < nBytes ; i++) {
		input[i] = output[i];
	}
	output = scrambler(input, nBytes , nCellId, output );
	// //Scrampler OUT PRINT
	if (print == 1) {
		printf("scrambler output from pbch start\n");
		for (i = 0; i < nBytes * 8; ++i)
		{
			printf("%x", getbit(output[i / 8], i % 8));
		}
		printf("\nscrambler output from pbch end\n");
	}


	//Modulation
	for (i = 0; i < nBytes ; i++) {
		input[i] = output[i];
	}
	if (modulation_scheme == 0)
		n_complex_array_length = ( nBytes * 8) / 2;
	else if (modulation_scheme == 1)
		n_complex_array_length = ( nBytes * 8) / 4;
	else if (modulation_scheme == 2)
		n_complex_array_length = ( nBytes * 8) / 6;

	cmplx_output = modulator( input, modulation_scheme, nBytes, cmplx_output);
	// //Modulation OUT PRINT
	if (print == 1) {
		printf("modulation output from pbch start\n");
		for (i = 0; i < nBytes * 8 / 2; ++i)
		{
			printf( "%.5f%+.5fi ",
			        creal(cmplx_output[i]), cimag(cmplx_output[i]));
		}
		printf("\nscrambler output from pbch end\n");
	}


	//Layermapping
	cmplx_input = (float complex*) malloc(n_complex_array_length*sizeof(float complex*));
	for (i = 0; i < n_complex_array_length ; i++) {
		cmplx_input[i] = cmplx_output[i];
	}
	layerMapper(cmplx_input,2,4,1,n_complex_array_length,cmplx_output);



	free(input);
	free(output);
}