#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
gsl_matrix_complex* precoder(float complex* input, int tr_scheme, int is_CDD, int n_antenna_port, int n_layers,int codebook_index,  int n_cmplx_length, gsl_matrix_complex* output);