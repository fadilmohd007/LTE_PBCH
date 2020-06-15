//gcc -g -cc -lm convolutionRateMatching.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
# include "convolutionRateMatching.h"



#define getbit(byte,nbit)   ((int) (byte >> nbit ) & 1)

uint8_t *tracker_initial;
uint8_t *tracker;

int get_coloumn(int i) {
	int array[32] = { 1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31,
	                  0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30
	                };
	return array[i];
}

//returns the index if present else -1
int is_present(int search, int length ) {
	int i;
	for (i = 0; i < length ; i++) {
		if (tracker_initial[i] == search)
			return i;
	}
	return -1;
}

uint8_t* convolutionRateMatcher(uint8_t* rateMatchInput, int nBytes, uint8_t* rateMatchOutput) {
	printf("=============================Convolution Rate Matching=================================\n");

	int j, i, k, index, E = 1920, Kw; // E value is 1920 for normal cyclic/ 1728 for extended from 36.211 6.6 scrambling bit length
	int n_coloumns = 4 , v = 0; //32/8bits = 4bytes

	//ceil of a number in int
	int n_rows = ceil(7 / 4);
	n_rows = 1 + ((nBytes - 1) / n_coloumns);


	int n_null_bytes = (n_rows * n_coloumns - nBytes);
	int n_trackerbits = n_null_bytes * 8 ;
	tracker_initial = (uint8_t *) malloc( n_trackerbits * sizeof(uint8_t));
	tracker = (uint8_t *) malloc(  n_trackerbits * sizeof(uint8_t));
	for (i = 0; i < n_trackerbits ; i++) {
		tracker_initial[i] =  0;
		tracker[i] = 0;
	}

	int n_null_bytes_filled = 0;
	int iter_input = 0, temp_coulumn;

	uint8_t interlever_matrix_0[n_rows][n_coloumns];
	uint8_t interlever_matrix_1[n_rows][n_coloumns];
	uint8_t interlever_matrix_2[n_rows][n_coloumns];
	uint8_t interlever_matrix_0_permuted[n_rows][n_coloumns];
	uint8_t interlever_matrix_1_permuted[n_rows][n_coloumns];
	uint8_t interlever_matrix_2_permuted[n_rows][n_coloumns];
	uint8_t interlever_matrix_0_permuted_out [n_rows * n_coloumns];
	uint8_t interlever_matrix_1_permuted_out [n_rows * n_coloumns];
	uint8_t interlever_matrix_2_permuted_out [n_rows * n_coloumns];
	uint8_t interlever_matrix_permuted_final_out_w [3 * n_rows * n_coloumns];
	rateMatchOutput =  ( uint8_t*) realloc(rateMatchOutput, (1920 / 8 ) * (sizeof(uint8_t)));

	for (i = 0; i < E / 8; i++) {
		rateMatchOutput[i] = 0;
	}
	for (i = 0; i < n_rows; ++i)
	{
		for ( j = 0; j < n_coloumns; ++j)
		{
			if (n_null_bytes_filled < n_null_bytes) {
				interlever_matrix_0[0][j] = -1;
				interlever_matrix_1[0][j] = -1;
				interlever_matrix_2[0][j] = -1;
				for (k = 0 ; k < 8 ; k++ ) {
					tracker_initial[n_null_bytes_filled * 8 + k] = n_null_bytes_filled * 8 +  k;
				}
				n_null_bytes_filled ++;
			}
			else {
				interlever_matrix_0[i][j] = rateMatchInput[iter_input];
				interlever_matrix_1[i][j] = rateMatchInput[nBytes + iter_input];
				interlever_matrix_2[i][j] = rateMatchInput[2 * nBytes + iter_input];
				iter_input++;
			}
		}
	}

	for ( i = 0; i < n_rows; ++i)
	{
		for ( j = 0; j < 4; ++j)
		{
			interlever_matrix_0_permuted[i][j] = 0;
			interlever_matrix_1_permuted[i][j] = 0;
			interlever_matrix_2_permuted[i][j] = 0;
		}
	}

	for ( i = 0; i < n_rows; ++i)
	{
		for ( j = 0; j < 32; ++j)
		{
			uint8_t temp_coulumn = get_coloumn(j);
			index = is_present(temp_coulumn , n_trackerbits );
			if ( i == 0 && index >= 0) {
				tracker[index] = j;
			}
			interlever_matrix_0_permuted[i][j / 8] = interlever_matrix_0_permuted[i][j / 8] | ( (getbit( interlever_matrix_0[i][temp_coulumn / 8], 7- (temp_coulumn % 8)) << (7-(j % 8)) ));
			interlever_matrix_1_permuted[i][j / 8] = interlever_matrix_1_permuted[i][j / 8] | ( (getbit( interlever_matrix_1[i][temp_coulumn / 8], 7- (temp_coulumn % 8)) << (7-(j % 8)) ));
			interlever_matrix_2_permuted[i][j / 8] = interlever_matrix_2_permuted[i][j / 8] | ( (getbit( interlever_matrix_2[i][temp_coulumn / 8], 7- (temp_coulumn % 8)) << (7-(j % 8)) ));
		}
	}
	for (i = 0; i < n_trackerbits; ++i)
	{
		tracker_initial[i] = tracker[i];
	}

	for ( i = 0; i < n_coloumns * n_rows; ++i)
	{
		interlever_matrix_0_permuted_out[i] = 0;
		interlever_matrix_1_permuted_out[i] = 0;
		interlever_matrix_2_permuted_out[i] = 0;

	}

	for ( i = 0; i < 32; ++i)
	{
		for ( j = 0; j < n_rows; ++j)
		{
			v = j + n_rows * i;

			index = is_present(i, n_trackerbits );
			if ( j == 0 && index >= 0) {
				tracker[index] = v;
			}
			interlever_matrix_0_permuted_out[ v / 8] = interlever_matrix_0_permuted_out [ v / 8] | ((getbit(interlever_matrix_0_permuted[j][ i / 8] , (7-(i % 8)) )) << (7-(v % 8)));
			interlever_matrix_1_permuted_out[ v / 8] = interlever_matrix_1_permuted_out [ v / 8] | ((getbit(interlever_matrix_1_permuted[j][ i / 8] , (7-(i % 8)) )) << (7-(v % 8)));
			interlever_matrix_2_permuted_out[ v / 8] = interlever_matrix_2_permuted_out [ v / 8] | ((getbit(interlever_matrix_2_permuted[j][ i / 8] , (7-(i % 8)) )) << (7-(v % 8)));
		}
	}

	for (i = 0; i < n_trackerbits; ++i)
	{
		tracker_initial[i] = tracker[i];
	}

	for ( i = 0; i < 3 * n_coloumns * n_rows; ++i)
	{
		interlever_matrix_permuted_final_out_w[i] = 0;
	}

	for ( i = 0; i < (n_rows * n_coloumns); ++i)
	{
		interlever_matrix_permuted_final_out_w[i] = interlever_matrix_0_permuted_out [i];
		interlever_matrix_permuted_final_out_w[(n_rows * n_coloumns) + i] = interlever_matrix_1_permuted_out [i];
		interlever_matrix_permuted_final_out_w[(n_rows * n_coloumns * 2) + i] = interlever_matrix_2_permuted_out [i];
	}


	k = 0;
	j = 0;
	int t = 0;
	while (k < E) {
		j = j % (n_rows * n_coloumns * 8 * 3);
		index = is_present((j % (8 * n_coloumns * n_rows)) , n_trackerbits);
		if ( !(index >= 0) ) {
			//for printing bits enable this
			//printf("%x", getbit(interlever_matrix_permuted_final_out_w[(j % (8 * n_coloumns * n_rows * 3)) / 8], (j % 8) ));
			rateMatchOutput[k / 8] = rateMatchOutput[k / 8] | (getbit(interlever_matrix_permuted_final_out_w[(j % (8 * n_coloumns * n_rows * 3)) / 8], (7-(j % 8)) ) << 7-(k % 8));
			k++;

		}
		j++;
	}

	for ( i = 0 ; i < E / 8 ; i++) {
		printf("%x ", rateMatchOutput[i] );
	}


	printf("\n=============================Convolution Rate Matching=================================\n");

	return rateMatchOutput;

}




// //tester
// int main() {
// 	uint8_t message[3*7] = {0xa6, 0x3b, 0x86, 0x24, 0x3b, 0xd8, 0x58, //10100110 00111011 10000110 00100100 00111011 11011000 01011000
// 	                          //{	0x65, 0xdc, 0x61, 0x24, 0xdc, 0x1b, 0x1a, //for dinesh  01100101 11011100 01100001 00100100 11011100 00011011 00011010
// 	                          0x3f, 0xf4, 0xe9, 0xc2, 0xc9, 0x41, 0x75,//
// 	                          //0xFC, 0x2F, 0x97, 0x43, 0x93, 0x82, 0xAe, //for dinesh  11111100 00101111 10010111 01000011 10010011 10000010 10101110
// 	                          0xa3, 0x76, 0xfe, 0xb0, 0x8f, 0xdd, 0xd1
// 	                          //0xC5, 0x6E, 0x7F, 0x0D, 0xF1, 0xBB, 0x8B,//for dinesh  11000101 01101110 01111111 00001101 11110001 10111011 10001011
// 	                         };
// 	uint8_t* rateMatchOutput;
// 	convolutionRateMatcher( (uint8_t *)message, 7, rateMatchOutput);
// }