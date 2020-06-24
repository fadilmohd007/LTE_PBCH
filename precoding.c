#include "precoding.h"
//globals
gsl_matrix_complex* codebook_two_antenna_ports, codebook_four_antenna;


void array_to_matrix(float complex* input, gsl_matrix_complex* input_matrix, int n_layers, int n_cmplx_length) {
	int i = 0, j = 0, k = n_cmplx_length / n_layers;
	gsl_complex* val;
	val = (gsl_complex*) malloc(sizeof(gsl_complex));
	for (i = 0 ; i < k; i++) {
		j = 0;
		while (j < n_layers) {
			GSL_SET_COMPLEX(&val[0], creal(input[i + j * k]), cimag(input[i + j * k])) ;
			gsl_matrix_complex_set(input_matrix , j, i, val[0] );
			j++;
		}
	}
}


void initialtecodebook() {
	gsl_complex* val;
	val = (gsl_complex*) malloc(sizeof(gsl_complex));
	codebook_two_antenna_ports = gsl_matrix_complex_alloc(8, 3);
	//for n_layers ==1
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 0, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 1, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 2, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], -1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 3, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 4, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, 1 / sqrt(2)) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 5, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 6, 0, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, -1 / sqrt(2)) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 7, 0, val[0]);
	// printf("%.5f%+ .5fi ", GSL_REAL( gsl_matrix_complex_get(codebook_two_antenna_ports, 7, 0)), GSL_IMAG( gsl_matrix_complex_get(codebook_two_antenna_ports, 7, 0)) );
	//for n_layers ==2 row ==1
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 0, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, 0);
	gsl_matrix_complex_set(codebook_two_antenna_ports, 1, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / 2, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 2, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / 2, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 3, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / 2, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 4, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, 1 / 2) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 5, 1, val[0]);
	//for n_layers ==2 row ==2
	GSL_SET_COMPLEX(&val[0], 0, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 0, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0);
	gsl_matrix_complex_set(codebook_two_antenna_ports, 1, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / 2, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 2, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], -1 / 2, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 3, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / 2, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 4, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, -1 / 2) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 5, 1, val[0]);
}


gsl_matrix_complex* precoder(float complex* input, int tr_scheme, int is_CDD, int n_antenna_port, int n_layers, int codebook_index,  int n_cmplx_length, gsl_matrix_complex* output) {
	printf("\n=============================Precoding=================================\n");
	int i, j, k;
	k = n_cmplx_length / n_layers;
	initialtecodebook();
	gsl_matrix_complex* input_matrix;
	input_matrix = gsl_matrix_complex_alloc(n_layers , k);
	output = gsl_matrix_complex_alloc(n_layers , k);
	array_to_matrix(input , input_matrix, n_layers, n_cmplx_length);

	if (tr_scheme == 0) {
		printf("precoding for single antenna port \n");
		for (i = 0 ; i < n_cmplx_length ; i++) {
			j = 0;
			while (j < 1) {
				gsl_matrix_complex_set(output, j, i, gsl_matrix_complex_get(input_matrix, j, i) );
				printf("%.5f%+ .5fi\n", GSL_REAL(gsl_matrix_complex_get(input_matrix, 0, i)), GSL_IMAG(gsl_matrix_complex_get(input_matrix, 0, i)) );
				j++;
			}
		}
	}


	else if (tr_scheme == 1) {
		if (is_CDD == 0) {
			printf("precoding for spatial multiplexing with cell specific ref signal without CDD\n");
			if (n_antenna_port == 2 && n_layers == 2 ) {
				gsl_matrix_complex_view W = gsl_matrix_complex_submatrix(codebook_two_antenna_ports, 0, 1, 2, 2);
				gsl_complex alpha,beta;
				GSL_SET_COMPLEX(&alpha,1,0);
				GSL_SET_COMPLEX(&beta,0,0);
				printf("%.5f%+ .5fi ", GSL_REAL( alpha), GSL_IMAG( alpha) );
				// printf("\n%d %d %d %d \n",input_matrix->size1,input_matrix->size2,(&W.matrix)->size1,(&W.matrix)->size2 );
				gsl_blas_zgemm (CblasNoTrans, CblasNoTrans ,alpha , &W.matrix,input_matrix,beta,output);


			}

		}
	}
	printf("\n=============================Precoding=================================\n");

}