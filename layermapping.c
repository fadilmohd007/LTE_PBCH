#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <complex.h>
#include "layermapping.h"



float complex* layerMapper(float complex* input , int tr_scheme, int n_layer, int n_codewords, int n_cmplx_array_length, float complex* output) {

	printf("=============================layermapping=================================\n");
	int i, k, j;
	k = n_cmplx_array_length / n_layer;
	if (tr_scheme == 0) { //layer mapping for single antenna port
		printf("layer mapping for single antenna port\n");
		for (i = 0 ; i < n_cmplx_array_length ; i++) {
			output[i] = input[i];
		}
		for (i = 0 ; i < n_cmplx_array_length ; i++) {
			printf( "%.5f%+ .5fi\n", creal(output[i]), cimag(output[i]));
		}
		printf("\n");

	}


	else if (tr_scheme == 1) { //layer mapping for spatial multiplexing
		printf("layer mapping for spatial multiplexing\n");
		if (n_codewords == 1) {
			output = (float complex*) realloc(output, sizeof(float complex) * n_cmplx_array_length);
			for ( i = 0; i < k ; i++) {
				output[i] = 0;
			}
			for ( i = 0; i < k ; i++) {
				j = 0;
				while (j < n_layer) {
					output[i + (j * k) ] = input[(n_layer * i) + j];
					j++;
				}

			}
		}
		for (i = 0 ; i < k; i++) {
			for (j = 0 ; j < n_layer ; j++) {
				printf( "%.5f%+ .5fi ", creal(output[i + (j * k)]), cimag(output[i + (j * k)]));
			}
			printf("\n");
		}

	}

	else if (tr_scheme == 2) { //layer mapping for transmit diversity
		printf("layer mapping for transmit diversity\n");
		if (n_cmplx_array_length % 4 != 0) {
			input = (float complex*) realloc(input, (n_cmplx_array_length + 2) * sizeof(float complex));
			input[n_cmplx_array_length] = 0;
			input[n_cmplx_array_length + 1] = 0;
		}
		if (n_codewords == 1) {
			output = (float complex*) realloc(output, sizeof(float complex) * n_cmplx_array_length);
			for ( i = 0; i < k ; i++) {
				output[i] = 0;
			}
			for ( i = 0; i < k ; i++) {
				j = 0;
				while (j < n_layer) {
					output[i + (j * k) ] = input[(n_layer * i) + j];
					j++;
				}

			}
		}
		for (i = 0 ; i < k; i++) {
			for (j = 0 ; j < n_layer ; j++) {
				printf( "%.5f%+ .5fi ", creal(output[i + (j * k)]), cimag(output[i + (j * k)]));
			}
			printf("\n");
		}

	}


	printf("\n=============================layermapping=================================\n");



	return output;


}
