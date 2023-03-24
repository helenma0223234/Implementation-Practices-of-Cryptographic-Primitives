// HW4 Q1 CBC MAC with DES                                                                                                                                                   
// DES implementation is provided by Prof. Smith

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "desW23.h"

#define BLOCKSIZE 8
#define IVSIZE 8

void get_cbc_mac (unsigned char message[], int message_len, unsigned char *key, unsigned char *iv, unsigned char *mac) {
    unsigned char* textbuffer = malloc(sizeof(char) * 8);
    unsigned char* outbuffer= malloc(sizeof(char) * 8);
    int block = 0;
    // // copy IV to be the initial block of ciphertext                                                                                                                     
    memcpy(iv, message, IVSIZE);
    // CBC iteration                                                                                                                                                     
    while (message_len >= BLOCKSIZE) {
        memcpy(textbuffer, message+BLOCKSIZE*block, BLOCKSIZE);
        // if (block == 2) {
        //     printf("this is the t XOR m2 blcok...\n");
        // }
        // XOR input w/ IV  
        if (block != 0) {
            for (int i = 0; i < BLOCKSIZE; i++) {
                textbuffer[i] = textbuffer[i] ^ iv[i];
            }
        }                                                                                                                                             
        // printf("XOR textbuffer: %s\n", bin_to_string(textbuffer,BLOCKSIZE));
        // encrypt current block                                                                                                                                         
        des_encrypt(textbuffer, key, outbuffer);

        // update IV w/ output                                                                                                                                           
        memcpy(iv, outbuffer, BLOCKSIZE);
        message_len -= BLOCKSIZE;
        block++;
    }
    memcpy(mac, outbuffer, BLOCKSIZE);
    free(textbuffer);
    free(outbuffer);
}

void forge_message(unsigned char m1[], int m1_size, unsigned char h1[], unsigned char m2[], int m2_size, unsigned char h2[], unsigned char m3[]) {
    // XORing the first block of m2 with h1 and concatenate the rest
    int out_ptr = 0;
    for (int i = 0; i < m1_size; i++) {
        m3[i] = m1[i];
        out_ptr++;
    }
    for (int i = 0; i<m2_size; i++) {
        if (i < BLOCKSIZE) {
            m3[out_ptr] = m2[i] ^ h1[i];
        } else {
            m3[out_ptr] = m2[i];
        }
        out_ptr++;
    }
}

int main(){
    unsigned char key[BLOCKSIZE] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59};
    unsigned char iv[] = "abcdefgh";

    // testing length extending attack
    unsigned char mac1[BLOCKSIZE];
    unsigned char m1[] = {0x02, 0x46, 0x8a, 0xce, 0xec, 0xa8, 0x64, 0x20, 0x6d, 0x73, 0x62, 0x72, 0x6f, 0x77, 0x6e, 0x33};
    size_t m1_size = sizeof(m1) / sizeof(char);
    printf("message1 (m1) is: %s\n", bin_to_string(m1,m1_size));
    printf("after CBC-MAC, mac of m1 is:\n");
    get_cbc_mac(m1, (int)m1_size, key, iv, mac1);
    printf("%s\n", bin_to_string(mac1,BLOCKSIZE));
    printf("\n");

    unsigned char mac2[BLOCKSIZE];
    unsigned char m2[] = {0x46, 0xce, 0xec, 0xa8, 0x6d, 0x62, 0x72, 0x6e, 0x02, 0x46, 0x8a, 0xce, 0xec, 0xa8, 0x64, 0x6e};
    size_t m2_size = sizeof(m2) / sizeof(char);
    printf("message2 (m2) is: %s\n", bin_to_string(m2,m2_size));
    get_cbc_mac(m2, (int)m2_size, key, iv, mac2);
    printf("mac2 is: %s\n", bin_to_string(mac2,BLOCKSIZE));
    printf("\n");

    int m3_size = m1_size + m2_size;
    unsigned char m3_forge[m3_size];
    
    unsigned char key2[BLOCKSIZE] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59};
    unsigned char iv2[] = "abcdefgh";
    unsigned char mac3[BLOCKSIZE];
    forge_message(m1,m1_size, mac1, m2, m2_size, mac2, m3_forge);
    printf("forged message message by concatenating m1 and m2...\n(m2's first block is XORed w/ m1's mac)\nmessag3(m3) is: %s\n", bin_to_string(m3_forge,m3_size));
    get_cbc_mac(m3_forge, m3_size, key2, iv2, mac3);
    printf("\n");
    printf("macs for m2 and forged m3 shoud be the same:\n");
    printf("mac for m2: %s\n", bin_to_string(mac2,BLOCKSIZE));
    printf("mac for m3: %s\n", bin_to_string(mac3,BLOCKSIZE));
}