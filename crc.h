#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TOPBIT_24 (0x800000)
#define TOPBIT_16 (0x8000)

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
uint8_t* crcSlow(uint8_t const message[], int nBytes,int polinomial, uint8_t* output);