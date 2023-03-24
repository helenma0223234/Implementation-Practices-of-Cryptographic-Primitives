#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define DEBUG 0
#define MAX_ROW 200000       // max number of lines

/* function head */
double check_uniformity(int* hash_frequency);

int md5_bhash(char *data, int len, int buckets) {
    unsigned char hval[128];
    unsigned int *ip = (unsigned int *) &hval[0], rval;
    
    MD5((const unsigned char *)data,len,&hval[0]);
    rval = (*ip) % buckets;
    if (DEBUG) {
        printf("md5_bhash %s to %d\n", data, rval);
    }
    return rval;
}

unsigned int cs10_bhash(char *data, int len, int buckets) {

  unsigned int sum,j,rval;

  if ( (0 == data) || (0 == len) ) {
    printf("illegal input\n");
    exit(-1);
  }
  sum = data[0];

  for (j = 1; j < len; j++) {
    sum = 37*sum + ((unsigned char)(data[j]));
  }

  rval = sum % buckets;

  if (DEBUG) {
    printf("cs10_bhash %s to %d\n", data, rval);

  }
  return rval;
}

void md5_hash_test(char** words) {
    int i, hash;
    for (i = 0; words[i] != NULL; i++) {
       hash = md5_bhash(words[i], strlen(words[i]), MAX_ROW-10);
       /*if (i < 20) {
           printf("%d", hash);
       }*/
    }
    printf("%d", hash);
}

void cs10_hash_test(char** words) {
    int i, hash;
    printf("\n running bucket hash func, printing first 20 pairs...\n");
    for (i = 0; words[i] != NULL; i++) {
       hash = cs10_bhash(words[i], strlen(words[i]), MAX_ROW-10);
        if (i < 20) {
            printf("%s ", words[i]);
            printf("%d\n", hash);
        }
    }
}

void md5_hash_distribution(char** words) {
    int hash_frequency[MAX_ROW] = {0};
    for (int i = 0; words[i] != NULL; i++) {
       int hash = md5_bhash(words[i], strlen(words[i]), MAX_ROW);
       hash_frequency[hash]++;
    }
    printf("for md5 hash, distribution of hashes is: %.6lf\n", check_uniformity(hash_frequency));
}

void cs10_hash_distribution(char** words) {
    int hash_frequency[MAX_ROW] = {0};
    for (int i = 0; words[i] != NULL; i++) {
       int hash = cs10_bhash(words[i], strlen(words[i]), MAX_ROW);
        hash_frequency[hash]++;
    }
    printf("for bucket hash, distribution of hashes is: %.6lf\n", check_uniformity(hash_frequency));
}

double check_uniformity(int* hash_frequency) {
    double expected = 1.0;
    double chi_sqr = 0;

    for (int i = 0; i < MAX_ROW; i++) {
        chi_sqr += pow(hash_frequency[i] - expected, 2) / expected;
    }

    return chi_sqr/(double)MAX_ROW;
}

char* freadLinep(FILE* fp)
{
  // validate the parameter
  if (fp == NULL || feof(fp)) {
    return NULL;
  }

  // allocate buffer big enough for "typical" words/lines
  int len = 81;
  char* buf = malloc(len * sizeof(char));
  if (buf == NULL) {
    return NULL;        // out of memory
  }

  // Read characters from file until newline or EOF, 
  // expanding the buffer when needed to hold more.
  int pos;
  char c;
  for (pos = 0; (c = fgetc(fp)) != EOF && (c != '\n'); pos++) {
    // We need to reserve buf[pos+1] for the terminating null
    // and buf[len-1] is the last usable slot, 
    // so if pos+1 is past that slot, we need to grow the buffer.
    if (pos+1 > len-1) {
      char* newbuf = realloc(buf, ++len * sizeof(char));
      if (newbuf == NULL) {
        free(buf);
        return NULL;        // out of memory
      } else {
        buf = newbuf;
      }
    }
    buf[pos] = c;
  }

  if (pos == 0 && c == EOF) {
    // no characters were read and we reached EOF
    free(buf);
    return NULL;
  } else {
    // pos characters were read into buf[0]..buf[pos-1].
    buf[pos] = '\0'; // terminate string
    return buf;
  }
}

int main() {
    int count = 0;

    FILE *fp = fopen("words", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    //int rows = 10;
    char** words = malloc(sizeof(char*) * MAX_ROW);

    char* line;
    while ( (line = freadLinep(fp)) != NULL && count < MAX_ROW) {
        //printf("[%s]\n", line);
        words[count] = line; //free(line);
        count++;
    }
    fclose(fp);

    /*for (int i = 0; i < count; i++) {
        printf("%s ", words[i]);
    }*/
    clock_t start, end;
    double elaps;
    
    start = clock();
    md5_hash_test(words);
    end = clock();
    elaps = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nMD5 time used: %f seconds\n", elaps);

    start = clock();
    cs10_hash_test(words);
    end = clock();
    elaps = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Bucket hahs time used: %f seconds\n", elaps);

    md5_hash_distribution(words);
    cs10_hash_distribution(words);
    return 0;
}
