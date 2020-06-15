//gcc -lm -o pbch pbch.c crc.o
#include <stdio.h>
#include <stdint.h>
#include "crc.h"
#include "tailBitConvolution.h"

int main() {

	int nBytes = 4;
	uint8_t message[7];
	uint8_t *crcoutput;
	int i = 0;

	message[0] = 0xB8;
	message[1] = 0x1F;
	message[2] = 0x2D;
	message[3] = 0x1C;
	//crc
	//message[4] = 0x27;
	// message[5] = 0x46;
	// message[6] = 0x38;

	crcoutput = crcSlow(message, nBytes, 1, crcoutput);
	nBytes = nBytes + 3;
	//CRC OUT PRINT
	printf("crcoutput from pbch start\n");
	for (i = 0; i < nBytes; ++i)
	{
		printf("%x", crcoutput[i] );
	}
	printf("\ncrcoutput from pbch end\n");
	crcoutput = tailBitEncoder(crcoutput, nBytes, crcoutput);
	//Encoder OUT PRINT
	printf("taibit output from pbch start\n");
	for (i = 0; i < nBytes * 3; ++i)
	{
		if (i % nBytes == 0   ) {
			printf("\n");
		}
		printf("%x", crcoutput[i] );
	}
	printf("\ntaibit output from pbch end\n");


}