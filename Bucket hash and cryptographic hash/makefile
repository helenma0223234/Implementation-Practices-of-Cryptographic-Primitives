CC = gcc
CFLAGS = -c -Wall -Werror

all: hashing

hashing: hashing.o
	$(CC) hashing.o -o hashing -lcrypto -lm -Wno-deprecated-declarations

hashing.o: hashing.c
	$(CC) $(CFLAGS) hashing.c -lcrypto -lm  -Wno-deprecated-declarations

clean:
	rm -f *.o elgamal hashing
