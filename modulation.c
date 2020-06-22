// gcc -c -lm  -std=c11 modulation.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#include "modulation.h"

#define getbit(byte,nbit)   ((int) (byte >> nbit ) & 1)

float complex * modulator(uint8_t * input, int modulationScheme, int nBytes, float complex* output) {
	printf("=============================Modulation=================================\n");
	int i;
	uint8_t temp;
	if (modulationScheme == 0 ) { //QPSK
		printf("QPSK modulation selected\n");
		int n_complex_array_length = ( nBytes * 8) / 2; // QPSK map 2 bits to 1 cmplx number
		output = (float  complex* ) malloc(n_complex_array_length * sizeof(float complex));
		for (i = 0 ; i < n_complex_array_length ; i++) {
			output[i] = ( (1 - (2 * (getbit(input[(i * 2) / 8], (i * 2) % 8))))  + I * (1 - (2 * (getbit(input[((i * 2) + 1) / 8], ((i * 2) + 1) % 8)) )) ) / sqrt(2);
			printf( "%.5f% + .5fi ", creal(output[i]), cimag(output[i]));
		}

	}
	else if (modulationScheme == 1) { //16QAM
		printf("16QAM modulation selected\n");
		float complex QAM16_modulation_matrix[16] = {CMPLX( (1 / sqrt(10)) , (1 / sqrt(10)) ),
		                                             CMPLX( (1 / sqrt(10)) , (3 / sqrt(10)) ),
		                                             CMPLX( (3 / sqrt(10)) , (1 / sqrt(10)) ),
		                                             CMPLX( (3 / sqrt(10)) , (3 / sqrt(10)) ),

		                                             CMPLX( (1 / sqrt(10)) , (-1 / sqrt(10)) ),
		                                             CMPLX( (1 / sqrt(10)) , (-3 / sqrt(10)) ),
		                                             CMPLX( (3 / sqrt(10)) , (-1 / sqrt(10)) ),
		                                             CMPLX( (3 / sqrt(10)) , (-3 / sqrt(10)) ),

		                                             CMPLX( (-1 / sqrt(10)) , (1 / sqrt(10)) ),
		                                             CMPLX( (-1 / sqrt(10)) , (3 / sqrt(10)) ),
		                                             CMPLX( (-3 / sqrt(10)) , (1 / sqrt(10)) ),
		                                             CMPLX( (-3 / sqrt(10)) , (3 / sqrt(10)) ),

		                                             CMPLX( (-1 / sqrt(10)) , (-1 / sqrt(10)) ),
		                                             CMPLX( (-1 / sqrt(10)) , (-3 / sqrt(10)) ),
		                                             CMPLX( (-3 / sqrt(10)) , (-1 / sqrt(10)) ),
		                                             CMPLX( (-3 / sqrt(10)) , (-3 / sqrt(10)) )
		                                            };

		int n_complex_array_length = ( nBytes * 8) / 4; // 16QAM map 4 bits to 1 cmplx number
		output = (float  complex* ) malloc(n_complex_array_length * sizeof(float complex));
		for (i = 0 ; i < n_complex_array_length ; i++) {
			temp = 0;
			temp = temp | getbit(input[(i * 4) / 8], (i * 4) % 8) << 3;
			temp = temp | getbit(input[(i * 4 + 1) / 8], (i * 4 + 1) % 8) << 2 ;
			temp = temp | getbit(input[(i * 4 + 2) / 8], (i * 4 + 2) % 8) << 1 ;
			temp = temp | getbit(input[(i * 4 + 3) / 8], (i * 4 + 3) % 8) ;
			output[i] = QAM16_modulation_matrix[temp];
			printf("%x ", temp );
		}

		for (i = 0 ; i < n_complex_array_length ; i++) {
			printf( "%.5f%+.5fi",
			        creal(output[i]), cimag(output[i]));
		}
	}
	else if (modulationScheme == 3) { //64QAM
		printf("====not implemented=======\n");
		exit(1);
	}

	printf("\n=============================Modulation=================================\n");
	return output;
}


// int main() {
// 	float complex z = CMPLX(1 / sqrt(2), -1 / sqrt(2));
//     // printf("z = %.5f% + .5fi\n", creal(z), cimag(z));
// 	uint8_t *temp;
// 	float complex* output;
// 	modulator(temp, 1, 4, output);

// }