//gcc -c crc.c

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "crc.h"
/*
 * The width of the CRC calculation and result.
 * Modify the typedef for a 16 or 32-bit CRC standard.
 */
typedef uint32_t crc;


#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (0x800000)


uint8_t* crcSlow(uint8_t const message[], int nBytes, int polinomial, uint8_t* output)
{
    printf("=============================CRC=================================\n");
    int i=0;
    uint32_t POLYNOMIAL = 0;
    uint32_t  remainder = 0;
    output = (uint8_t *)malloc(sizeof(uint8_t) * (nBytes+3));
    if (polinomial == 1) { //24a
        POLYNOMIAL = 0x864CFB;
    }
    else if (polinomial == 2) { //24b
        POLYNOMIAL = 0x800063;
    }

    int byte = 0;
    for ( byte = 0; byte < nBytes;)
    {

        remainder = remainder & 0xFFFFFF;
        uint32_t  temp = 0;
        if (nBytes % 3 == 2 && byte == 0) {
            printf("PADDING 1 BYTE OF ZERO IN STARTING\n");
            temp = (temp | 0x0) << 8; //basically no change
            temp = (temp | message[byte]) << 8; //first input
            byte++;
            temp = (temp | (message[byte])); //second input
            byte++;
        } else if (nBytes % 3 == 1 && byte == 0) {
            printf("PADDING 2 BYTES OF ZERO IN STARTING\n");
            temp = (temp | 0x0) << 8; //basically no change
            temp = (temp | 0x0) << 8; //basically no change
            temp = (temp | (message[byte])); //first input
            byte++;
        }
        else {
            temp = (temp | message[byte]) << 8;
            byte++;
            temp = (temp | (message[byte])) << 8;
            byte++;
            temp = (temp | (message[byte]));
            byte++;
        }
        remainder = remainder ^ temp;

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        uint8_t bit;
        for ( bit = 24; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (remainder & TOPBIT)
            {

                remainder = (remainder  << 1) ^ POLYNOMIAL;
                remainder = remainder & 0x00ffffff;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /*
     * The final remainder is the CRC result.
     */
    // uint8_t output[3] = {0,0,0};
    for ( i = 0; i < nBytes; ++i)
    {
        output[i] = message[i];
    }
    output[nBytes+0] = (remainder & 0xff0000) >> 16;
    output[nBytes+1] = (remainder & 0xff00) >> 8;
    output[nBytes+2] = (remainder & 0xff);
    printf("calulated crc with(1-24a,2-24b)\n polinomial %d \n crc %x %x %x\n", polinomial, output[nBytes+0], output[nBytes+1], output[nBytes+2]);
    printf("=============================CRC=================================\n");
    return output;

}   /* crcSlow() */



//sample function call for crc below and two exmaples

// uint8_t message[4];
//DINESH SAMPLE 1 FROM MAIL ANS =439F52
// message[0] = 0x58;
// message[1] = 0x1d;
// message[2] = 0x2c;
// message[3] = 0x5a;
//dINESH SAMPLE 2 FROM MAIL ANS=0X274638
// message[0] = 0xB8;
// message[1] = 0x1F;
// message[2] = 0x2D;
// message[3] = 0x1C;
// uint8_t *output;
// output = crcSlow(message,4,1,output);
// printf("(crc = %x)\n",output );

