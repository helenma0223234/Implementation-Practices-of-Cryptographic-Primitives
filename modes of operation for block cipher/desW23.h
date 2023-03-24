#ifndef __DESW23_H
#define __DESW23_H

#include <stdio.h>
#include <stdlib.h>

char* bin_to_string(const unsigned char input[], int nbytes);

void print_hex(unsigned char input[], unsigned char nbytes);

// void xor(const unsigned char* block_a, const unsigned char* block_b, unsigned char* output, unsigned char nbytes);

void permute(const unsigned char* input, const unsigned char* table,
             unsigned char* output, unsigned char nbytes);

void des_encrypt(unsigned char block[8], unsigned char key[8], unsigned char output[8]);

void des_decrypt(unsigned char block[8], unsigned char key[8], unsigned char output[8]);


#endif //__DESW23_H
