#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <complex.h>
#include "crc.h"
#include "tailBitConvolution.h"
#include "convolutionRateMatching.h"
#include "scrambling.h"
#include "modulation.h"
#include "layermapping.h"
#include "precoding.h"
#include "RE_mapping.h"
#include "../global_vars.h"

gsl_complex* pbch_main(	int nBytes, uint8_t message[], int nCellId, int n_layers , int n_codewords, int modulation_scheme,
                int transmission_scheme, int is_CDD , int n_antennaports, int code_bookindex, gsl_complex* lte_frame, int n_RB, int CP);