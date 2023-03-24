// HW2 Q4 raw RSA
// by Yuchuan Ma

/* sample output:
read 3432 primes 
Enter indices for two primes:
1254
2001
p = 10243, q = 17401
n = 178238443, phi = 178210800
e = 7
d = 101834743
e = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 
d = 0 0 0 1 1 0 0 0 0 1 0 0 0 1 1 1 0 1 1 1 1 1 1 1 1 1 0 1 1 1 
n = 0 0 1 0 1 0 1 0 0 1 1 1 1 1 1 0 1 1 0 0 1 1 1 1 1 0 1 0 1 1 

Enter message: 
234
c = m^e mod n = 100004567
c^d mod n = 234

Enter message: */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define PQSIZE 15
#define OUTSIZE 31
#define PRIMESIZE 3432

// function head declare
long extendedEuclidean(long a, long b, long* x2, long* y2);
void SquareAndMult(unsigned long m, unsigned long e, int n, unsigned long long* result);
void print_bi(unsigned n);

// function implementations
void rsa_init(int p_idx, int q_idx, long primes[], unsigned long* e, unsigned long* n, unsigned long* d) {
    unsigned long p = primes[p_idx], q = primes[q_idx];

    // calculate n, phi
    *n = p * q;
    unsigned long phi = (p-1)*(q-1);

    // derive an e that works
    srandom(time(NULL));
    // int limit = p+q;
    int r = rand() % (p+q);
    // r = arc4random_uniform (limit);
    while (r > PRIMESIZE || primes[r] >= phi) {
        r = rand() % (p+q);
    }
    
    *e = primes[r];
    // *e = 7;
    // calculate d
    long x,y;
    *d = extendedEuclidean(*e, phi, &x, &y);

    // print parameters
    printf("p = %ld", p);
    printf(", q = %ld\n", q);
    printf("n = %ld", *n);
    printf(", phi = %ld\n", phi);
    printf("e = %ld\n", *e);
    printf("d = %ld\n", *d);
    // print in binary format
    printf("e = ");
    print_bi(*e);
    printf("d = ");
    print_bi(*d);
    printf("n = ");
    print_bi(*n);
}

void rsa_encrypt(unsigned long plaintext, unsigned long e, unsigned long n, unsigned long long* encrypted) {
    // C ^ e mod N
    // square-and-multiply method
    SquareAndMult(plaintext,e,n, encrypted);
    printf("c = m^e mod n = %llu\n", *encrypted);
}

void rsa_decrypt(unsigned long long ciphertext, unsigned long d, unsigned long n, unsigned long long *decrypted) {
    // c^d mod n
    SquareAndMult(ciphertext,d,n, decrypted);
    printf("c^d mod n = %llu\n", *decrypted);
}

long extendedEuclidean(long a, long b, long* x, long* y) {
    *x = 1, *y = 0; //x2, y2
    if (b == 0) return a;

    long x1 = 0, y1 = 1, x2 = 1, y2 = 0, a1 = a, b1 = b;
    long q, r;
    while (b1>0) {
        // long q = a1 / b1;
        // long temp = *x2;
        // *x2 = x1;
        // x1 = temp - q * x1;
        // temp = *y2;
        // *y2 = y1;
        // y1 = temp - q * y1;
        // temp = a1;
        // a1 = b1;
        // b1 = temp - q * b1;
        // printf("%ld\n", a1);
        q = a1 / b1;
        r = a1 - q*b1;
        *x = x2 - q*x1;
        *y = y2 - q*y1;
        a1 = b1;
        b1 = r;
        x2 = x1;
        x1 = *x;
        y2 = y1;
        y1 = *y;
    }
    *x = x2;
    *y = y2;
    long d = *x;
    // x is the private key, but if it is negative, increment it by phi
    while (d < 0) {
        d += b;
    }
    return d;
}

// reference: the craft of system security section 7.7.1
void SquareAndMult(unsigned long m, unsigned long e, int n, unsigned long long* result) {
    *result = 1;
    while (e != 0) {
        // multiply
        if ((e & 0x1) == 1){       // reference: https://www.youtube.com/watch?v=4DmGuMQUrwc&t=129s
            *result = (*result*m)%n;
        }
        // squaring
        m = (m*m)%n;
        e = e >> 1;
    }
}

// reference: https://www.geeksforgeeks.org/binary-representation-of-a-given-number/
void print_bi(unsigned n) {
    unsigned i;
    for (i = 1 << OUTSIZE; i > 0; i = i / 2)
        {(n & i) ? printf("1 ") : printf("0 ");}
    printf("\n");
}

int main() {
    long primes[PRIMESIZE];
    int count = 0;
    long* p_list = primes;

    // read in prime list file
    FILE *file = fopen("primelist.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    while(fscanf(file, "%ld", &primes[count]) != EOF) {
        count++;
    }
    printf("read %d primes\n", count);
    fclose(file);

    int idx0, idx1;
    printf("Enter indices for two primes:\n");
    scanf("%d", &idx0);
    scanf("%d", &idx1);
    while (idx0 > PRIMESIZE || idx1 > PRIMESIZE) {
        printf("Enter indices for two primes between 0 - 3432:\n");
        scanf("%d", &idx0);
        scanf("%d", &idx1);
    }

    // init the key
    unsigned long e,n,d;
    rsa_init(idx0, idx1, p_list, &e, &n, &d);

    // using long long so the rsa work for longer input
    unsigned long long plaintext, ciphertext, plaintext1;
    while(1) {
        printf("Enter message:\n");
        scanf("%lld", &plaintext);
        rsa_encrypt(plaintext, e, n, &ciphertext);
        rsa_decrypt(ciphertext, d, n, &plaintext1);
    }
}
