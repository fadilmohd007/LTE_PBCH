//gcc -c  -I//usr/local/include/ precoding.c
#include "precoding.h"
//globals
gsl_matrix_complex* codebook_two_antenna_ports;
gsl_complex u_matrix[16][4];

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

gsl_matrix_complex* derrive_W_four_antenna(int codebook_index, int n_layers , gsl_matrix_complex *W) {
	int i, j;
	gsl_complex alpha, beta;
	gsl_matrix_complex *Un_matrix, *identity, *out1_matrix, *out2_matrix;
	Un_matrix = gsl_matrix_complex_alloc(4, 1);
	for (i = 0; i < 4; i++) {
		gsl_matrix_complex_set(Un_matrix, i, 0 , u_matrix[codebook_index][i]);
	}

	identity = gsl_matrix_complex_alloc(4, 4);
	out1_matrix = gsl_matrix_complex_alloc(4, 4);
	out2_matrix = gsl_matrix_complex_alloc(1, 1);
	gsl_matrix_complex_set_identity(identity);
	GSL_SET_COMPLEX(&alpha, 2, 0);
	GSL_SET_COMPLEX(&beta, 0, 0);
	gsl_blas_zgemm(CblasNoTrans, CblasTrans, alpha, Un_matrix, Un_matrix, beta, out1_matrix);
	GSL_SET_COMPLEX(&alpha, 1, 0);
	gsl_blas_zgemm(CblasConjTrans, CblasNoTrans, alpha, Un_matrix, Un_matrix, beta, out2_matrix);
	gsl_matrix_complex_scale(out1_matrix, (gsl_complex_div(alpha, gsl_matrix_complex_get(out2_matrix, 0, 0))) );
	gsl_matrix_complex_sub(identity, out1_matrix);
	if (n_layers == 1) {
		W = gsl_matrix_complex_alloc(4, 1);
		gsl_vector_complex *v;
		v = gsl_vector_complex_alloc(4);
		gsl_matrix_complex_get_col(v, identity, 3);
		gsl_matrix_complex_set_col(W, 0, v);
		gsl_vector_complex_free(v);

	}
	gsl_matrix_complex_free(out1_matrix);
	gsl_matrix_complex_free(out2_matrix);
	gsl_matrix_complex_free(Un_matrix);
	gsl_matrix_complex_free(identity);
	return W;

}

void initialtecodebook() {
	int i, j;
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
	//for n_layers ==2 row ==1
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 0, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, 0);
	gsl_matrix_complex_set(codebook_two_antenna_ports, 1, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], .5, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 2, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], .5, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 3, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], .5, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 4, 1, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, .5) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 5, 1, val[0]);
	//for n_layers ==2 row ==2
	GSL_SET_COMPLEX(&val[0], 0, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 0, 2, val[0]);
	GSL_SET_COMPLEX(&val[0], 1 / sqrt(2), 0);
	gsl_matrix_complex_set(codebook_two_antenna_ports, 1, 2, val[0]);
	GSL_SET_COMPLEX(&val[0], .5, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 2, 2, val[0]);
	GSL_SET_COMPLEX(&val[0], -.5, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 3, 2, val[0]);
	GSL_SET_COMPLEX(&val[0], .5, 0) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 4, 2, val[0]);
	GSL_SET_COMPLEX(&val[0], 0, -.5) ;
	gsl_matrix_complex_set(codebook_two_antenna_ports, 5, 2, val[0]);
	// printf("%.5f%+ .5fi ", GSL_REAL( gsl_matrix_complex_get(codebook_two_antenna_ports, 2, 1)), GSL_IMAG( gsl_matrix_complex_get(codebook_two_antenna_ports, 2, 1)) );
	// printf("%.5f%+ .5fi ", GSL_REAL( gsl_matrix_complex_get(codebook_two_antenna_ports, 2, 2)), GSL_IMAG( gsl_matrix_complex_get(codebook_two_antenna_ports, 2, 2)) );
	// printf("%.5f%+ .5fi ", GSL_REAL( gsl_matrix_complex_get(codebook_two_antenna_ports, 3, 1)), GSL_IMAG( gsl_matrix_complex_get(codebook_two_antenna_ports, 3, 1)) );
	// printf("%.5f%+ .5fi ", GSL_REAL( gsl_matrix_complex_get(codebook_two_antenna_ports, 3, 2)), GSL_IMAG( gsl_matrix_complex_get(codebook_two_antenna_ports, 3, 2)) );
	printf("\n");

	//for four antenna ports
	// u_matrix = (gsl_complex*) malloc(4*16*sizeof(gsl_complex));
	gsl_complex temp_u_matrix[16][4] =   	 		{ gsl_complex_rect(1, 0), gsl_complex_rect(-1, 0), 					  gsl_complex_rect(-1, 0), gsl_complex_rect(-1, 0),					//0
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(0, -1), 					  gsl_complex_rect(1, 0),  gsl_complex_rect(0, 1),					//1
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(1, 0), 					  gsl_complex_rect(-1, 0), gsl_complex_rect(1, 0),					//2
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(0, 1), 					  gsl_complex_rect(1, 0),  gsl_complex_rect(0, -1),					//3
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(-1 / sqrt(2), -1 / sqrt(2)), gsl_complex_rect(0, -1), gsl_complex_rect(1 / sqrt(2), -1 / sqrt(2)),	//4
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(1 / sqrt(2), -1 / sqrt(2)),  gsl_complex_rect(0, 1),  gsl_complex_rect(-1 / sqrt(2), -1 / sqrt(2)), //5
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(1 / sqrt(2), 1 / sqrt(2)),   gsl_complex_rect(0, -1), gsl_complex_rect(-1 / sqrt(2), 1 / sqrt(2)),	//6
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(-1 / sqrt(2), 1 / sqrt(2)),  gsl_complex_rect(0, 1),  gsl_complex_rect(1 / sqrt(2), 1 / sqrt(2)),	//7
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(-1, 0), 					  gsl_complex_rect(1, 0),  gsl_complex_rect(1, 0),					//8
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(0, -1), 					  gsl_complex_rect(-1, 0), gsl_complex_rect(0, -1),					//9
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(1, 0), 					  gsl_complex_rect(1, 0),  gsl_complex_rect(-1, 0),					//10
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(0, 1), 					  gsl_complex_rect(-1, 0), gsl_complex_rect(0, 1),					//11
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(-1, 0), 					  gsl_complex_rect(-1, 0), gsl_complex_rect(1, 0),					//12
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(-1, 0), 					  gsl_complex_rect(1, 0),  gsl_complex_rect(-1, 0),					//13
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(1, 0), 					  gsl_complex_rect(-1, 0), gsl_complex_rect(-1, 0),					//14
	                                                  gsl_complex_rect(1, 0), gsl_complex_rect(1, 0), 					  gsl_complex_rect(1, 0),  gsl_complex_rect(1, 0)					//15

	                                         };
	memcpy(u_matrix, temp_u_matrix, sizeof(gsl_complex) * 16 * 4);
	// for ( i = 0; i < 16; ++i)
	// {
	// 	for ( j = 0; j < 4; ++j)
	// 	{
	// 		printf("%.5f%+ .5fi ", GSL_REAL(u_matrix[i][j]), GSL_IMAG(u_matrix[i][j]) );
	// 	}
	// 	printf("\n");
	// }

}


gsl_matrix_complex* precoder(float complex* input, int tr_scheme, int is_CDD, int n_antenna_port, int n_layers, int codebook_index,  int n_cmplx_length, gsl_matrix_complex* output) {
	printf("\n=============================Precoding=================================\n");
	int i, j, k;
	k = n_cmplx_length / n_layers;
	initialtecodebook();
	gsl_matrix_complex* input_matrix;
	input_matrix = gsl_matrix_complex_alloc(n_layers , k);
	output = gsl_matrix_complex_alloc(n_antenna_port , k);
	array_to_matrix(input , input_matrix, n_layers, n_cmplx_length);

	if (tr_scheme == 0) {
		printf("precoding for single antenna port \n");
		if (n_antenna_port != 1 ) {
			printf("invalid antenna port for single antenna transmission\n");
			return;
		}
		for (i = 0 ; i < k ; i++) {
			j = 0;
			while (j < n_layers) {
				gsl_matrix_complex_set(output, j, i, gsl_matrix_complex_get(input_matrix, j, i) );
				printf("%.5f%+ .5fi\n", GSL_REAL(gsl_matrix_complex_get(input_matrix, j, i)), GSL_IMAG(gsl_matrix_complex_get(input_matrix, j, i)) );
				j++;
			}
		}
		return output;
	}


	else if (tr_scheme == 1) {
		if (is_CDD == 0) {
			printf("precoding for spatial multiplexing with cell specific ref signal without CDD\n");
			if (n_antenna_port == 2 ) {
				printf("number of antenna ports is  %d\n", n_antenna_port);
				if (n_layers == 2 && codebook_index == 3) {
					printf("invalid codebook index exiting\n");
					return;
				}
				gsl_matrix_complex_view W = gsl_matrix_complex_submatrix(codebook_two_antenna_ports, 2 * codebook_index, n_layers - 1, 2, n_layers);
				gsl_complex alpha, beta;
				GSL_SET_COMPLEX(&alpha, 1, 0);
				GSL_SET_COMPLEX(&beta, 0, 0);
				gsl_blas_zgemm (CblasNoTrans, CblasNoTrans , alpha , &W.matrix, input_matrix, beta, output);
				for (i = 0 ; i < k ; i++) {
					j = 0;
					while (j < n_antenna_port) {
						printf("%.5f%+ .5fi ", GSL_REAL(gsl_matrix_complex_get(output, j, i)), GSL_IMAG(gsl_matrix_complex_get(output, j, i)) );
						j++;
					}
					printf("\n");
				}
			}
			else if (n_antenna_port == 4) {
				printf("number of antenna ports is %d\n", n_antenna_port);
				gsl_matrix_complex *W;
				gsl_complex alpha, beta;
				GSL_SET_COMPLEX(&alpha, 1, 0);
				GSL_SET_COMPLEX(&beta, 0, 0);
				W =  derrive_W_four_antenna(codebook_index, n_layers, W);
				gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, alpha, W, input_matrix, beta, output);
				for (i = 0 ; i < k ; i++) {
					j = 0;
					while (j < n_antenna_port) {
						printf("%.5f%+ .5fi ", GSL_REAL(gsl_matrix_complex_get(output, j, i)), GSL_IMAG(gsl_matrix_complex_get(output, j, i)) );
						j++;
					}
					printf("\n");
				}
				return output;
			}
		}
		else if(is_CDD == 1){
			printf("precoding for spatial multiplexing with cell specific ref signal with CDD is not implemented exiting\n");
			return;
		}
	}
	else{
		printf("precoding for only spatial multiplexing is  implemented, exiting\n");
			return;
	}
	printf("\n=============================Precoding=================================\n");

}