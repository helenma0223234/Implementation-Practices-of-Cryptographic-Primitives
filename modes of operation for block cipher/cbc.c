// HW1 Q5 CBC with DES
// DES implementation is provided by Prof. Smith

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "desW23.h"

#define BLOCKSIZE 8
#define IVSIZE 8

void encrypt_message(unsigned char message[], unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {

    int block = 0;
    int out_ptr = IVSIZE; // skip the IV at the head
    unsigned char* textbuffer = malloc(8);
    unsigned char* outbuffer= malloc(8);
    int message_len = strlen((char*) message);
    // padding if needed since each block is 8 bit
    int pad = message_len % 8;
    if (pad != 0) {
        for (int i = 0; i < 8-pad; i++) {
            message[message_len+i] = '\0';
        }
        message_len += 8-pad;
    }
    // copy IV to be the initial block of ciphertext
    memcpy(ciphertext, iv, IVSIZE);

    // CBC iteration
    while (message_len >= BLOCKSIZE) {
        memcpy(textbuffer, message+BLOCKSIZE*block, BLOCKSIZE);
        
        // XOR input w/ IV
        for (int i = 0; i < BLOCKSIZE; i++) {
            textbuffer[i] = textbuffer[i] ^ iv[i];
        }
        // encrypt current block
        des_encrypt(textbuffer, key, outbuffer);
        
        // update IV w/ output
        memcpy(iv, outbuffer, BLOCKSIZE);

        // copy out ciphertext
        for (int j = 0; j < BLOCKSIZE; j++) {
            ciphertext[out_ptr] = outbuffer[j];
            out_ptr++;
        }
        //printf("%s\n", bin_to_string(outbuffer, 8));

        message_len -= BLOCKSIZE;
        block++;
    }
    free(textbuffer);
    free(outbuffer);
}

void decrypt_message(unsigned char ciphertext[], unsigned char *key, unsigned char *plaintext2) {

    int block = 0;
    int out_ptr = 0;
    unsigned char* textbuffer = malloc(8);
    unsigned char* outbuffer= malloc(8);

    unsigned char iv[IVSIZE];
    memcpy(iv, ciphertext, IVSIZE);

    int message_len = strlen((char*) ciphertext) - IVSIZE;

    // CBC iteration
    while (message_len >= BLOCKSIZE) {
        memcpy(textbuffer, ciphertext+BLOCKSIZE*block + IVSIZE, BLOCKSIZE);  // 8 is to pad IV

        // decrypt
        des_decrypt(textbuffer, key, outbuffer);

        // XOR bottom register with top content, and copy it out to output
        for (int j = 0; j < BLOCKSIZE; j++) {
            outbuffer[j] = outbuffer[j] ^ iv[j];
            plaintext2[out_ptr] = outbuffer[j];
            out_ptr++;
        }
        // update IV w/ cipher
        memcpy(iv, textbuffer, BLOCKSIZE);

        message_len -= BLOCKSIZE;
        block++;
    }

    free(textbuffer);
    free(outbuffer);
}


int main() {
    unsigned char plaintext[] = {0x02, 0x46, 0x8a, 0xce, 0xec, 0xa8, 0x64, 0x20, 0x6d, 0x73, 0x62, 0x72, 0x6f, 0x77, 0x6e, 0x33, 0x22};
    int plain_len = strlen((char *) plaintext);
    unsigned char key[8] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59};
    unsigned char ciphertext[plain_len+IVSIZE];
    unsigned char plaintext2[plain_len];

    unsigned char iv[] = "abcdefgh";

    encrypt_message(plaintext, key, iv, ciphertext);
    
    decrypt_message(ciphertext, key, plaintext2);

    printf("len 17 char array plaintext test1\n");
    printf("plaintext input:\n");
    printf("%s\n", bin_to_string(plaintext,plain_len));
    printf("ciphertext:\n");
    printf("%s\n", bin_to_string(ciphertext,plain_len+IVSIZE));
    printf("decrypted plaintext:\n");
    printf("%s\n", bin_to_string(plaintext2,plain_len));
}
