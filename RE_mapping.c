#include "RE_mapping.h"

/*
 * Function: RE_mapping_pbch
 * ----------------------------
 *   Returns an int with exit status.
 *	 Function fills an entire 10 mis frame in lte with PBCH precoder output
 *	 Checks if refernece signal is present in that slot if no? then fill with PBCH
 * 	 PBCH will be filled in n_slot = 1  and l=0,1,2,3 and k ranging from k = n_RB * n_RB_SC/2 -36 +k' where k ' = 0 , 1 ,..., 71
 *
 *
 *   CP          				int
 *	 lte_frame 					gsl_complex*
 *	 gsl_matrix_cmplx_output	gsl_matrix_complex*
 *	 n_RB						int
 *	 n_antenna_port				int
 *
 *   returns: with exit status
 */

int RE_mapping_pbch(gsl_matrix_complex* gsl_matrix_cmplx_output, gsl_complex* lte_frame, int CP, int n_RB, int n_antenna_port ) {
	printf("============================= RE Mapping PBCH =================================\n");
	//local and iteration variables
	int i, j, precoder_iter = 1, p;
	int n_DL_SYM, k, lte_frame_pos;

	if (CP == 1 ) {
		n_DL_SYM = 7;//Normal CP
	}
	else {
		n_DL_SYM = 6;//extended CP
	}
	for (p = 0; p < n_antenna_port; p++) {
		for (i = 0; i < 4; ++i)//for l from 0-3
		{
			for (j = 0; j < 72; ++j)//for k_dash from 0 to 71
			{
				k = (n_RB * 6) - 36 + j;
				lte_frame_pos = get_lte_frame_pos(1, 7, k, i, 0 );
				if (is_gsl_complex_zero(lte_frame[lte_frame_pos])) {// slot-10, k-602, l-0
					lte_frame[lte_frame_pos] = gsl_matrix_complex_get(gsl_matrix_cmplx_output, p, precoder_iter);
					printf("PBCH filled in sf-0 ,iteration_j-%d, slot-1, k-%d, l-%d, lte_pos-%d which is", j, k, i, lte_frame_pos);
					printf("%.5f + %.5f \n", GSL_REAL(lte_frame[lte_frame_pos]), GSL_IMAG(lte_frame[lte_frame_pos]) );
					precoder_iter++;
				}
				else {
					printf("PBCH not filled in sf-0 ,iteration_j-%d, slot-1, k-%d, l-%d, lte_pos-%d which was", j, k, i, lte_frame_pos);
					printf("%.5f + %.5f \n", GSL_REAL(lte_frame[lte_frame_pos]), GSL_IMAG(lte_frame[lte_frame_pos]) );
				}
			}
		}
	}
	printf("============================= RE Mapping PBCH =================================\n");
	return 0;
}

