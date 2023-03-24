// HW1 Q5 CTR with DES
// DES implementation is provided by Prof. Smith

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "desW23.h"

#define BLOCKSIZE 8

void encrypt_message(unsigned char message[], unsigned char *key, unsigned char nonce[], unsigned char *ciphertext) {
    
    int out_ptr = 0; 

    unsigned char* top_res = malloc(8);
    unsigned char* bottom_res = malloc(8);

    int message_len = strlen((char*) message);

    // padding the nonce to 64 bit
    for (int i = 0; i < 4; i++) {
            nonce[4+i] = '\0';
        }
    //printf(bin_to_string(nonce,8));
    
    // padding the plain text if needed since each block needs 8 bit
    int pad = message_len % 8;
    if (pad != 0) {
        for (int i = 0; i < 8-pad; i++) {
            message[message_len+i] = '\0';
        }
        message_len += 8-pad;
    }

    // copy IV to be the initial block of ciphertext                                                                                          
    memcpy(top_res, nonce, BLOCKSIZE);

    // CTR iteration                                                                                                                          
    while (message_len >= BLOCKSIZE) {
        // encrypt the top, output to bottom                                                                                                       
        des_encrypt(top_res, key, bottom_res);

        // XOR bottom register with plain, and copy it out to output                                                                                                            
        for (int j = 0; j < BLOCKSIZE; j++) {
            ciphertext[out_ptr] = bottom_res[j] ^ message[out_ptr];
            out_ptr++;
        }

        // increment the counter
        top_res[7]++;
        message_len -= BLOCKSIZE;
    }
    free(top_res);
    free(bottom_res);
}

void decrypt_message(unsigned char ciphertext[], unsigned char *key, unsigned char nonce[], unsigned char *plaintext2) {
    encrypt_message(ciphertext, key, nonce, plaintext2);
}

int main() {
    unsigned char plaintext[] = {0x02, 0x46, 0x8a, 0xce, 0xec, 0xa8, 0x64, 0x20, 0x6d, 0x73, 0x62, 0x72, 0x6f, 0x77, 0x6e, 0x33};
    unsigned char plaintext_short[] = {0x02, 0x46, 0x8a, 0xce, 0xec, 0xa8, 0x64, 0x20, 0x6d, 0x73};    // needs padding
    int plain_len = strlen((char *) plaintext);
    int plain_len2 = strlen((char *) plaintext_short);
    unsigned char nonce[4] = {0x0f, 0x15, 0x71, 0xc9};
    unsigned char key[8] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59};

    unsigned char ciphertext[plain_len];
    unsigned char plaintext2[plain_len];

    unsigned char ciphertext_short[plain_len2];
    unsigned char plaintext2_short[plain_len2];

    
    // first plaintext test
    encrypt_message(plaintext, key, nonce, ciphertext);

    decrypt_message(ciphertext, key,nonce, plaintext2);

    // second test
    encrypt_message(plaintext_short, key, nonce, ciphertext_short);
    decrypt_message(ciphertext_short, key, nonce, plaintext2_short);

    
    printf("testing with len 16 plaintext char array:\n");
    printf("plain text:\n");
    printf("%s\n", bin_to_string(plaintext,plain_len));
    printf("ciphertext:\n");
    printf("%s\n", bin_to_string(ciphertext,plain_len));
    printf("decrypted plaintext:\n");
    printf("%s\n", bin_to_string(plaintext2,plain_len));
    printf("\n");

    printf("testing with len 6 plaintext char array:\n");
    printf("short plain text w/ padding:\n");
    printf("%s\n", bin_to_string(plaintext_short,plain_len2));
    printf("ciphertext:\n");
    printf("%s\n", bin_to_string(ciphertext_short,plain_len2));
    printf("decrypted plaintext:\n");
    printf("%s\n", bin_to_string(plaintext2_short,plain_len2));
}
