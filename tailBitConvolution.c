#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tailBitConvolution.h"






uint8_t* tailBitEncoder(uint8_t* encod_input , int nBytes, uint8_t* encoded_output ) {
	printf("=============================tailBitEncoder=================================\n");
	int i, j;
	uint8_t shift_reg = encod_input[nBytes - 1];
	uint8_t current_input_byte, current_output_0_byte , current_output_1_byte , current_output_2_byte, current_input_bit, current_output_0_bit, current_output_1_bit, current_output_2_bit ;
	shift_reg = shift_reg & 0x3F; //(in binary= 0011 1111) final shift_reg is nibble swap of needed one / which is compremised with the algorithm

	encoded_output = (uint8_t*) realloc(encoded_output,nBytes * 3 * (sizeof(uint8_t)));


	for (i = 0 ; i < nBytes ; i++ ) {
		current_input_byte = encod_input[i];
		current_output_0_byte = 0;
		current_output_1_byte = 0;
		current_output_2_byte = 0;
		for (j = 7; j >= 0; j--)
		{
			current_input_bit = (current_input_byte >> j) & 1;
			current_output_0_bit = current_input_bit ^ ((shift_reg >> 1) & 1) ^ ((shift_reg >> 2) & 1) ^ ((shift_reg >> 4) & 1) ^ ((shift_reg >> 5 ) & 1);
			current_output_1_bit = current_input_bit ^ ((shift_reg) & 1) ^ ((shift_reg >> 1) & 1) ^ ((shift_reg >> 2) & 1) ^ ((shift_reg >> 5) & 1);
			current_output_2_bit = current_input_bit ^ ((shift_reg) & 1) ^ ((shift_reg >> 1) & 1) ^ ((shift_reg >> 3) & 1) ^ ((shift_reg >> 5 ) & 1);
			current_output_0_byte = current_output_0_byte | (current_output_0_bit << j);
			current_output_1_byte = current_output_1_byte | (current_output_1_bit << j);
			current_output_2_byte = current_output_2_byte | (current_output_2_bit << j);
			shift_reg = shift_reg << 1;
			shift_reg = (shift_reg | (current_input_bit));
		}
		encoded_output[i] = current_output_0_byte;
		encoded_output[i + nBytes] = current_output_1_byte;
		encoded_output[i + nBytes * 2] = current_output_2_byte;

	}
	for (i = 0; i < nBytes * 3; ++i)
	{
		if (i % nBytes == 0   ) {
			printf("\n");
		}
		printf("%x", encoded_output[i] );

	}
	printf("\n");

	printf("=============================tailBitEncoder=================================\n");

	return encoded_output;


}


//tester
// int main() {
// 	uint8_t crc_out[7];
// 	uint8_t *encoded_output;

// 	crc_out[0] = 0xB8;
// 	crc_out[1] = 0xC2;
// 	// crc_out[1] = 0x1F;
// 	crc_out[2] = 0x2D;
// 	crc_out[3] = 0x1C;
// 	crc_out[4] = 0x27;
// 	crc_out[5] = 0x46;
// 	crc_out[6] = 0x78;

// // expected encoded_output :
// // 	0:5ecf
// // 	1:1768
// // 	2:6bfc


// 	tailBitEncoder(crc_out, 2, encoded_output);

// }