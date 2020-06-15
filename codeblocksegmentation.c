//gcc -lm -o cbc codeblocksegmentation.c turbo_code_table.o crc.o 
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "turbo_code_table.h"
#include "crc.h"


#define Z_max_size 6144

void  segmentation_and_crc_attach(uint8_t input[],int nBytes){


	uint8_t L_crc_bits =0;
	uint8_t C_number_of_codeblocks =0;
	uint8_t B_number_input_bits = nBytes *8;
	uint8_t B_dash_number_output_bits = 0 ;
	uint8_t K_plus_segementation_size = 0;
	uint8_t C_plus_number_of_segments = 0;
	uint8_t K_minus_segementation_size = 0;
	uint8_t C_minus_number_of_segments = 0;
	uint8_t Delta_k = 0;
	uint8_t F_filler_bits = 0;
	uint8_t F_filler_bytes =0;
	uint8_t k_iterator =0, s_iterator =0, r_iterator =0, K_selector;
	int i,j;
	uint8_t out[1][20];
 	for ( j = 0; j < 20; ++j)
	{
		out[0][j] = 0;
	}	
	
	//dertermining number of codeblocks
	if(B_number_input_bits <= Z_max_size){
		L_crc_bits =0;
		C_number_of_codeblocks =1;
		B_dash_number_output_bits = B_number_input_bits;
	}
	else{
		L_crc_bits = 24;
		C_number_of_codeblocks = ceil( B_number_input_bits/(Z_max_size-L_crc_bits));
		B_dash_number_output_bits = B_number_input_bits + C_number_of_codeblocks*L_crc_bits;
	}

	if(C_number_of_codeblocks == 1){
		K_plus_segementation_size = get_K_plus(C_number_of_codeblocks ,B_dash_number_output_bits );
		C_plus_number_of_segments = 1;
	}
	else if(C_number_of_codeblocks>1){
		K_plus_segementation_size = get_K_plus(C_number_of_codeblocks ,B_dash_number_output_bits );
		K_minus_segementation_size = get_K_minus(K_plus_segementation_size);
		Delta_k = K_plus_segementation_size -K_minus_segementation_size;
		C_minus_number_of_segments = floor( (( (C_number_of_codeblocks * K_plus_segementation_size) - B_dash_number_output_bits)/Delta_k) );
		C_plus_number_of_segments = C_number_of_codeblocks - C_minus_number_of_segments;
	}					
	F_filler_bits = (C_plus_number_of_segments * K_plus_segementation_size) + (C_minus_number_of_segments * K_minus_segementation_size) -B_dash_number_output_bits;



	F_filler_bytes = F_filler_bits /8;

	for (k_iterator = 0; k_iterator < F_filler_bytes;  k_iterator++)
	{	
		printf("%d th byte is 0 coz of filler bits",k_iterator);
		 out[0][k_iterator] = 0;
	}

	k_iterator = F_filler_bytes;
	s_iterator =0;
	r_iterator =0;

	for (r_iterator = 0; r_iterator < (C_number_of_codeblocks); ++r_iterator)
	{
		if(r_iterator < C_minus_number_of_segments)
			K_selector = K_minus_segementation_size;
		else
			K_selector = K_plus_segementation_size;

		while(k_iterator < (K_selector -L_crc_bits)/8){
			out[r_iterator][k_iterator] = input[ s_iterator];
			k_iterator = k_iterator+1;
			s_iterator = s_iterator +1;
		}
		if(C_number_of_codeblocks = 1){
		printf("calculate the crc and append\n");
		uint8_t *crcoutput;
		crcoutput =crcSlow(out[r_iterator],k_iterator,2,crcoutput);
		}

		//debug
		for ( j = 0; j < 20; ++j)
		{
			printf("%x ", out[r_iterator][j] );
		}
	}




}




int main(){
	uint8_t message[7];
    message[0] = 0xB8;
 	message[1] = 0x1F;
    message[2] = 0x2D;
    message[3] = 0x1C;
    message[4] = 0x27;
	message[5] = 0x46;
	message[6] = 0x38;
    segmentation_and_crc_attach(message,7);
    // printf("%d",check());
}