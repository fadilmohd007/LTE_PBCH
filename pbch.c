//gcc -lm -o pbch pbch.c crc.o convolutionRateMatching.o tailBitConvolution.o
#include <stdio.h>
#include <stdint.h>
#include "crc.h"
#include "tailBitConvolution.h"
#include "convolutionRateMatching.h"
#include <stdlib.h>

int main() {

	int nBytes = 4;
	uint8_t message[7];
	uint8_t *output,*input;
	int i = 0;

	message[0] = 0xB8;
	message[1] = 0x1F;
	message[2] = 0x2D;
	message[3] = 0x1C;
	//crc
	//message[4] = 0x27;
	// message[5] = 0x46;
	// message[6] = 0x38;

	output = crcSlow(message, nBytes, 1, output);
	nBytes = nBytes + 3;
	//CRC OUT PRINT
	// printf("output from pbch start\n");
	// for (i = 0; i < nBytes; ++i)
	// {
	// 	printf("%x", output[i] );
	// }
	// printf("\noutput from pbch end\n");
	output = tailBitEncoder(output, nBytes, output);
	//Encoder OUT PRINT
	// printf("taibit output from pbch start\n");
	// for (i = 0; i < nBytes * 3; ++i)
	// {
	// 	if (i % nBytes == 0   ) {
	// 		printf("\n");
	// 	}
	// 	printf("%x", output[i] );
	// }
	// printf("\ntaibit output from pbch end\n");

	input = (uint8_t*) malloc(nBytes*3*sizeof(uint8_t));
	for (i = 0; i < nBytes * 3; i++) {
		input[i] = output[i];
	}
	output = convolutionRateMatcher(input , nBytes , output );
	// //Ratematching OUT PRINT
	// printf("ratematching output from pbch start\n");
	// for (i = 0; i < 1920/8; ++i)
	// {

	// 	printf("%x ", output[i] );
	// }
	// printf("\ntaibit output from pbch end\n");
	printf("%d\n",0%8 );
}