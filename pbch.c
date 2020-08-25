//gcc -lm -lgsl -lgslcblas -o pbch pbch.c crc.o convolutionRateMatching.o tailBitConvolution.o scrambling.o modulation.o layermapping.o precoding.o
//export LD_LIBRARY_PATH=/usr/local/lib
#include "pbch.h"

gsl_complex* pbch_main(	int nBytes, uint8_t message[], int nCellId, int n_layers , int n_codewords, int modulation_scheme,
                        int transmission_scheme, int is_CDD , int n_antennaports, int code_bookindex, gsl_complex* lte_frame,
                        int n_RB, int CP) {
	//config
	int print = 1;

	//local and iterrators
	uint8_t *output, *input;
	int i = 0;



	float complex *cmplx_output, *cmplx_input;
	int n_complex_array_length;

	gsl_matrix_complex* gsl_matrix_cmplx_output, gsl_cmplx_input;


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
	output = crcSlow(message, nBytes, 3, output);
	nBytes = nBytes + 2;
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
		for (i = 0; i < n_complex_array_length; ++i)
		{
			printf( "%.5f%+.5fi ",
			        creal(cmplx_output[i]), cimag(cmplx_output[i]));
		}
		printf("\nscrambler output from pbch end\n");
	}


	//Layermapping
	cmplx_input = (float complex*) malloc(n_complex_array_length * sizeof(float complex*));
	for (i = 0; i < n_complex_array_length ; i++) {
		cmplx_input[i] = cmplx_output[i];
	}
	cmplx_output =  layerMapper(cmplx_input, transmission_scheme, n_layers, n_codewords, n_complex_array_length, cmplx_output);
	//LayerMapping OUT PRINT
	if (print == 1) {
		int k = n_complex_array_length / n_layers, j;
		printf("layermapping output from pbch start\n");
		for (i = 0 ; i < k; i++) {
			for (j = 0 ; j < n_layers ; j++) {
				printf( "%.5f%+ .5fi ", creal(cmplx_output[i + (j * k)]), cimag(cmplx_output[i + (j * k)]));
			}
			printf("\n");
		}
		printf("\nlayermapping output from pbch end\n");
	}


	//precoding
	for (i = 0; i < n_complex_array_length ; i++) {
		cmplx_input[i] = cmplx_output[i];
	}
	gsl_matrix_cmplx_output = precoder( cmplx_input, transmission_scheme, is_CDD, n_antennaports, n_layers, code_bookindex , n_complex_array_length,  gsl_matrix_cmplx_output);
	int j;
	RE_mapping_pbch( gsl_matrix_cmplx_output,  lte_frame,  CP, n_RB, n_antennaports, nCellId);


	free(cmplx_output);
	free(cmplx_input);
	free(input);
	free(output);
	return  lte_frame;
}