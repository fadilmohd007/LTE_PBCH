//gcc -c crc.c
#include "crc.h"
/*
 * Function: Append CRC parity bits to the input stream of data
 * ----------------------------
 *   Returns an array of uint8_t with appended parity bits (crc24 -3 bytes crc16 - 2 bytes)
 *   The output pointer passed to the function is modified to an array and appended the crc accordingly
 *
 *
 *   message[]              uint8_t
 *   nBytes                 int
 *   polinomial             int(selector of crc generator)
 *   output               uint8_t
 *
 *   returns: pointer array of uint8_t of message appended with crc
 */

uint8_t* crcSlow(uint8_t const message[], int nBytes, int polinomial, uint8_t* output)
{
    printf("=============================CRC=================================\n");
    int i = 0;
    uint32_t POLYNOMIAL = 0;
    uint32_t  remainder = 0;

    if (polinomial == 1) { //24a
        POLYNOMIAL = 0x864CFB;
        output = (uint8_t *)malloc(sizeof(uint8_t) * (nBytes + 3));
    }
    else if (polinomial == 2) { //24b
        POLYNOMIAL = 0x800063;
        output = (uint8_t *)malloc(sizeof(uint8_t) * (nBytes + 3));
    }
    else if (polinomial == 3) { //16
        POLYNOMIAL = 0x1021;
        output = (uint8_t *)malloc(sizeof(uint8_t) * (nBytes + 2));
    }
    if (polinomial == 1 || polinomial == 2) {
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
                if (remainder & TOPBIT_24)
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
    }

    else if (polinomial == 3) {
        int byte = 0;
        for ( byte = 0; byte < nBytes;)
        {
            remainder = remainder & 0xFFFF;
            uint32_t  temp = 0;
            if (nBytes % 2 == 1 && byte == 0) {
                printf("PADDING 1 BYTE OF ZERO IN STARTING\n");
                temp = (temp | 0x0) << 8; //basically no change
                temp = (temp | message[byte]); //first input
                byte++;
            }
            else {
                temp = (temp | message[byte]) << 8;
                byte++;
                temp = (temp | (message[byte]));
                byte++;
            }
            remainder = remainder ^ temp;

            /*
             * Perform modulo-2 division, a bit at a time.
             */
            uint8_t bit;
            for ( bit = 16; bit > 0; --bit)
            {
                /*
                 * Try to divide the current data bit.
                 */
                if (remainder & TOPBIT_16)
                {
                    remainder = (remainder  << 1) ^ POLYNOMIAL;
                    remainder = remainder & 0xffff;
                }
                else
                {
                    remainder = (remainder << 1);
                }
            }
        }
    }
    /*
     * The final remainder is the CRC result.
     */
    for ( i = 0; i < nBytes; ++i)
    {
        output[i] = message[i];
    }
    if (polinomial == 1 || polinomial == 2) {

        output[nBytes + 0] = (remainder & 0xff0000) >> 16;
        output[nBytes + 1] = (remainder & 0xff00) >> 8;
        output[nBytes + 2] = (remainder & 0xff);
        printf("calulated crc with(1-24a,2-24b,3-16)\n polinomial %d \n crc %x %x %x\n", polinomial, output[nBytes + 0], output[nBytes + 1], output[nBytes + 2]);
    }
    else if (polinomial == 3) {
        output[nBytes + 0] = (remainder & 0xff00) >> 8;
        output[nBytes + 1] = (remainder & 0xff);
        printf("calulated crc with(1-24a,2-24b,3-16)\n polinomial %d \n crc %x %x\n", polinomial, output[nBytes + 0], output[nBytes + 1]);
    }
    printf("=============================CRC=================================\n");
    return output;
}   /* crcSlow() */



//sample function call for crc 24a
//dINESH SAMPLE 2 FROM MAIL ANS=0X274638
// message[0] = 0xB8;
// message[1] = 0x1F;
// message[2] = 0x2D;
// message[3] = 0x1C;


//sample function call for crc 16
//dINESH SAMPLE 2 FROM MAIL ANS=0XB847
// message[0] = 0xB8;
// message[1] = 0x1F;
// message[2] = 0x2D;
// message[3] = 0x1C;

