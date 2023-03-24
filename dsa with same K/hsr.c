#include <stdio.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>


#define BLEN 100

/*
  Sample code to:
    - read in a hash file and a signature file
    - and turn them into bignums

  sws, feb 2023

 */


int main(int argc, char *argv[]) {

 FILE  *hashfile, *sigfile;
 DSA_SIG *dsig;
 const BIGNUM *r, *s;
 BIGNUM *m = BN_new();
 

 unsigned char hashbuf[BLEN];
 unsigned char sigbuf[BLEN];
 int hashlen, siglen;
 int rc;
 const unsigned char *cp,**cpp;

 if( argc < 3) {
   printf("needs arguments: hashfile sigfile\n");
   return -1;
 }


 hashfile = fopen(argv[1],"r");
 sigfile = fopen(argv[2],"r"); 
 hashlen = fread(hashbuf,1,BLEN,hashfile);
 siglen = fread(sigbuf,1,BLEN,sigfile);

 if (hashlen != 20)
   printf("warning: hash is not a sha-1 hash?\n");
 
 BN_bin2bn(hashbuf, hashlen, m);
 printf("hash as a bignum  %s\n",BN_bn2hex(m));

 cp = &sigbuf[0];
 cpp = &cp;
 
 dsig = d2i_DSA_SIG(NULL,cpp,siglen);

 if (NULL == dsig) {



   printf("warning: signature file was not properly formatted\n");
   return -1;
 }
 
 DSA_SIG_get0(dsig,&r,&s);
 printf("r %s\n",BN_bn2hex(r));
 printf("s %s\n",BN_bn2hex(s));
  
}



