#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <string.h>
#include <math.h>
#include "../global_vars.h"	
#include <gsl/gsl_blas.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>


#define is_gsl_complex_zero(gsl_cmplx_num) (( GSL_REAL(gsl_cmplx_num) * 1  == 0 && GSL_IMAG(gsl_cmplx_num)*1== 0)?1:0)

int RE_mapping_pbch(gsl_matrix_complex* gsl_matrix_cmplx_output, gsl_complex* lte_frame, int CP, int n_RB,  int n_antenna_port, int nCellId);