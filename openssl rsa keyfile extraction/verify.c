#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/decoder.h>
#include <openssl/bn.h>

/* functions to verify key parameters */


int main() {
    OSSL_DECODER_CTX *dctx = NULL;
    EVP_PKEY *pkey = NULL;
    const char *format = "PEM";
    const char *structure = NULL;
    const char *keytype = "RSA";
    BIGNUM *e = NULL, *d = NULL, *n = NULL, *p = NULL, *q = NULL;
    FILE *pemfile;
    BN_CTX *ctx = BN_CTX_new();

    // construct the decoder obj
    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, format, structure, keytype, OSSL_KEYMGMT_SELECT_KEYPAIR, NULL, NULL);
    if (dctx == NULL) {
        printf("can't create decoder\n");
        return 1;
    }

    // open key.pem file for reading
    if (!(pemfile = fopen("key.pem", "r"))) {
        printf("failed to open key.pem\n");
        return 1;
    }

    // read the key pair from the pem file
    if (!OSSL_DECODER_from_fp(dctx, pemfile)) {
        return 1;
    }

    // get each components and print them out
    // get e
    if (!EVP_PKEY_get_bn_param(pkey, "e", &e)) {
        return 1;
    }
    //char* e_dec = BN_bn2dec(e);
    printf("all in hexcode...\npublic key e:\n");
    BN_print_fp(stdout, e);
    printf("\n");
    // get n
    if (!EVP_PKEY_get_bn_param(pkey, "n", &n)) {
        return 1;
    }
    printf("modulus n:\n");
    BN_print_fp(stdout, n);
    printf("\n");
    // get p,q
    if(!EVP_PKEY_get_bn_param(pkey, "rsa-factor1", &p)) {
		printf("no p found");
		return 1;
	} else {
		printf("prime p:\n");
		BN_print_fp(stdout, p);
		printf("\n");
	}
	if(!EVP_PKEY_get_bn_param(pkey, "rsa-factor2", &q)) {
		printf("no q found");
		return 1;
	} else {
		printf("q:\n");
		BN_print_fp(stdout, q);
		printf("\n");
	}
    // get d
    if (!EVP_PKEY_get_bn_param(pkey, "d", &d)) {
        return 1;
    }
    printf("private key d:\n");
    BN_print_fp(stdout, d);
    printf("\n");

    // verity n = pq, 
    // and ed = 1 mod (p-1)(q-1)
    printf("\nchecking if n = p*q...\n");
    BIGNUM *res = BN_new();
    // Multiply p and q and store the result
    BN_mul(res, p, q, ctx);
    int check_pq = BN_cmp(res, n);
    if (check_pq == 0) {
        printf("pq = n\n");
    } else {
        printf("pq != n\n");
    }
    
    printf("checking e and d...\n");
    BIGNUM* d_check = BN_new();
    //BN_mul(ed, e, d, ctx);
    BIGNUM *p_minus_one = BN_new();
    BIGNUM *q_minus_one = BN_new();
    BN_sub(p_minus_one, p, BN_value_one());     //p-1
    BN_sub(q_minus_one, q, BN_value_one());     //q-1
    BIGNUM *phi = BN_new();
    BN_mul(phi, p_minus_one, q_minus_one, ctx);
    BN_mod_inverse(d_check, e, phi, ctx);
    int check_d = BN_cmp(d_check,d);
    if (check_d == 0) {
        printf("d is the mod inverse of e mod phi (p-1)(q-1)\n");
    } else {
        printf("d is not the mod interse\n");
    }

    // clean up
    EVP_PKEY_free(pkey);
    OSSL_DECODER_CTX_free(dctx);
    BN_free(p_minus_one);
    BN_free(q_minus_one);
    BN_free(phi);
    BN_free(d_check);
    BN_free(res);
    BN_free(e);
    BN_free(d);
    BN_free(n);
    BN_free(p);
    BN_free(q);
    return 0;
}
