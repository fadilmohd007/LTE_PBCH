//gcc -c scrambling.c
#include "scrambling.h"

#define getbit(byte,nbit)   ((int) (byte >> nbit ) & 1)

uint8_t* scrambler(uint8_t* scramblerInput, int nBytes, int nCellId, uint8_t* scramblerOutput) {
	printf("=============================Scambling PBCH=================================\n");

	uint8_t  *c;
	int i;

	c = gen_gold_sequence(nCellId, nBytes, c);
	for (i = 0 ; i < nBytes  ; i++) {
		scramblerOutput[i] = 0;
	}

	for (i = 0 ; i < nBytes * 8 ; i++) {
		scramblerOutput[i / 8] = scramblerOutput[i / 8] | ( getbit(scramblerInput[i / 8] , i % 8) ^ getbit(c[i / 8], i % 8 )) << i % 8;
	}


	for ( i = 0; i < nBytes * 8; ++i)
	{
		printf("%x", getbit(scramblerInput[i / 8], i % 8));
	}
	printf("\n");

	for ( i = 0; i < nBytes * 8; ++i)
	{
		printf("%x", getbit(scramblerOutput[i / 8], i % 8));
	}
	printf("\n");

	// for ( i = 0; i < nBytes; ++i)
	// {
	// 	printf("%x ", scramblerOutput[i ]);
	// }
	// printf("\n");

	free(c);
	printf("=============================Scambling PBCH=================================\n");
	return scramblerOutput;

}