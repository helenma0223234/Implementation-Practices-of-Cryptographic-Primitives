// CS62 hw4 Q3 calculate private key with w/ pub key and two same-k pair of texts
// by Yuchuan Ma
#include <stdlib.h>
#include <stdio.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>
#include <openssl/decoder.h>
#include <openssl/encoder.h>
#include <openssl/param_build.h>
#include <openssl/evp.h>
#include <openssl/core_names.h>
#include <openssl/bn.h>

#define BLEN 100

/* 
    Sample code provided by Prof Sean Smith, 
    reads in dsa pem file
    examine the parameters, create a new dsa keypair based on that
    NOTE: small changes made for the purpose of this assignment. 
*/
void get_dsa_params(char* pemfile_name, BIGNUM *p, BIGNUM *q, BIGNUM *g, BIGNUM *y, BIGNUM *a) {
    FILE *pemfile;
    //int rc;
    OSSL_DECODER_CTX *dctx;
    EVP_PKEY *pkey = NULL;
    BIGNUM *q_temp= NULL, *p_temp = NULL, *g_temp=NULL, *y_temp = NULL;
    pemfile = fopen(pemfile_name, "r");

    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, "PEM", NULL, "DSA",0,NULL, NULL);
    if (NULL == dctx) {
        printf("OSSL_DECODER_CTX_new_for_pkey failed\n");
        return;
    }
    if (0 == OSSL_DECODER_from_fp(dctx, pemfile)) {
        printf("OSSL_DECODER_from_fp failed\n");
        return;
    }

    if (EVP_PKEY_get_bn_param(pkey, "priv", &a))
        printf("a: %s\n",BN_bn2hex(a));
    if (EVP_PKEY_get_bn_param(pkey, "pub",  &y_temp))
        printf("y: %s\n",BN_bn2hex(y_temp));      
    if (EVP_PKEY_get_bn_param(pkey, "p",    &p_temp))
        printf("p: %s\n",BN_bn2hex(p_temp));
    if (EVP_PKEY_get_bn_param(pkey, "q",    &q_temp))
        printf("q: %s\n",BN_bn2hex(q_temp));
    if (EVP_PKEY_get_bn_param(pkey, "g",    &g_temp))
        printf("g: %s\n",BN_bn2hex(g_temp));
    BN_copy(q,q_temp);
    BN_copy(g,g_temp);
    BN_copy(y,y_temp);
    BN_copy(p,p_temp);
    fclose(pemfile);
}

/* 
    Sample code provided by Prof Sean Smith, 
    reads in hash file and signature file,
    reads their value into BIGNUM
    NOTE: small changes made for the purpose of this assignment. 
*/
void get_bn_hash_and_sig(char* hashfile1, char* sigfile, const BIGNUM *r, const BIGNUM *s, BIGNUM *m) {
    FILE *hashfile_ptr, *sigfile_ptr;
    DSA_SIG *dsig;
    const BIGNUM *s_temp, *r_temp;
    unsigned char hashbuf[BLEN];
    unsigned char sigbuf[BLEN];
    int hashlen, siglen;
    const unsigned char *cp,**cpp;

    hashfile_ptr = fopen(hashfile1,"r");
    sigfile_ptr = fopen(sigfile,"r");
    if (hashfile_ptr == NULL || sigfile_ptr == NULL) {
        printf("Error opening files!\n");
        return;
    }
    hashlen = fread(hashbuf,1,BLEN,hashfile_ptr);
    siglen = fread(sigbuf,1,BLEN,sigfile_ptr);

    if (hashlen != 20) {
        printf("warning: hash is not a sha-1 hash?\n");
        return;
    }
    BN_bin2bn(hashbuf, hashlen, m);
    printf("hash as a bignum: %s\n",BN_bn2hex(m));

    cp = &sigbuf[0];
    cpp = &cp;
    dsig = d2i_DSA_SIG(NULL,cpp,siglen);

    if (NULL == dsig) {
        printf("warning: signature file was not properly formatted\n");
        return;
    }
    DSA_SIG_get0(dsig,&r_temp,&s_temp);
    BN_copy(s,s_temp);
    BN_copy(r,r_temp);
    printf("r: %s\n",BN_bn2hex(r));
    printf("s: %s\n",BN_bn2hex(s));
    fclose(hashfile_ptr);
    fclose(sigfile_ptr);
}

int main() {
    // get BN of message hashes and signatures
    BIGNUM *r = BN_new(),*r2 = BN_new(), *s = BN_new(), *s2 = BN_new(), *m = BN_new(), *m2 = BN_new();
    printf("Parsing messages' hashes and signatures into BIGNUMs...\n");
    get_bn_hash_and_sig("msg1.hash","msg1.sign", r, s, m);
    get_bn_hash_and_sig("msg2.hash", "msg2.sign", r2, s2, m2);

    // get DSA pub key params
    printf("Parsing out dsa public key parameters...\n");
    BIGNUM *p = BN_new(), *q = BN_new(), *g = BN_new(), *y = BN_new(), *a = NULL;
    get_dsa_params("dsa_pub.pem", p, q, g, y, a);

    BN_CTX *ctx = BN_CTX_new();

    // to calculate k, after a little algebra...
    // we just do k = (h1-h2)/(s1-s2) where h is the hash value
    BIGNUM *h_diff = BN_new(), *s_diff = BN_new(), *k = BN_new(), *s_diff_inv = BN_new();
    BN_sub(h_diff, m, m2);
    BN_sub(s_diff, s, s2);
    BN_mod_inverse(s_diff_inv, s_diff,q,ctx);
    BN_mod_mul(k, h_diff, s_diff_inv,q, ctx);
    //BN_div(k, NULL, h_diff, s_diff, ctx);
    printf("\nnow we calculate k = (h2-h1)/(s2-s1),\nk is:%s\n", BN_bn2hex(k));

    printf("now calculate private key with x = ((s * k) - h) * r-1 mod q\n");
    BIGNUM *inv_mod_r = BN_new(), *sk = BN_new(), *sk_minus_m = BN_new(), *priv = BN_new();
    BN_mod_inverse(inv_mod_r, r, q, ctx);
    //BN_mod_mul(sk, s, k, q, ctx);
    BN_mul(sk, s, k, ctx);
    BN_sub(sk_minus_m, sk, m);
    BN_mod_mul(priv, sk_minus_m, inv_mod_r, q, ctx);
    printf("private key is: %s\n", BN_bn2hex(priv));
    
    // use the calculated private key to generate a private key pem file
    FILE *pemfile2;
    BIGNUM *y2=BN_new(),*p2=BN_new(),*q2=BN_new(),*g2=BN_new();
    EVP_PKEY_CTX *pctx = NULL;
    EVP_PKEY *pkey2 = NULL;
    OSSL_ENCODER_CTX *ectx;
    OSSL_PARAM_BLD *tmpl = NULL;
    OSSL_PARAM *params = NULL;


    //BN_copy(a2,priv);  // the guess
    BN_copy(p2,p);
    BN_copy(y2,y);
    BN_copy(q2,q);
    BN_copy(g2,g);
    //printf("copied g2 is %s\"\n", BN_bn2hex(g2));

    //--------------------
    // this part adapted from https://github.com/openssl/openssl/blob/5540855bda5e58f4b33f2306feb6ff4e93c0af81/apps/testdsa.h#L239-L268
    if ((pctx = EVP_PKEY_CTX_new_from_name(NULL, "DSA", NULL)) == NULL)  {
        printf("trouble\n");
        return -1;
    }

    if ((tmpl = OSSL_PARAM_BLD_new()) == NULL
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_FFC_P,
                                    p2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_FFC_Q,
                                    q2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_FFC_G,
                                    g2)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_PRIV_KEY,
                                    priv)
        || !OSSL_PARAM_BLD_push_BN(tmpl, OSSL_PKEY_PARAM_PUB_KEY,
                                    y2)
        || (params = OSSL_PARAM_BLD_to_param(tmpl)) == NULL) {
        printf("more trouble\n");
        return -1;
    }

    if (EVP_PKEY_fromdata_init(pctx) <= 0
        || EVP_PKEY_fromdata(pctx, &pkey2, EVP_PKEY_KEYPAIR,
                                params) <= 0) {
        printf("still more trouble\n");
        return -1;
        
    }
    //----------------------

    ectx = OSSL_ENCODER_CTX_new_for_pkey(pkey2, EVP_PKEY_KEYPAIR, "PEM", NULL, NULL);

    if (NULL == ectx) {
        printf("even more trouble\n");
        return -1;
    }

    pemfile2 = fopen("test_private_key.pem","w");

    if (0==OSSL_ENCODER_to_fp(ectx,pemfile2)) {
        printf("OSSL_ENCODER_to_fp failed\n");
        return -1;
    }

    return 0;
}
