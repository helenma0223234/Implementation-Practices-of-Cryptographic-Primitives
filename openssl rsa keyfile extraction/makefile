CC = gcc
CFLAGS = -c -Wall -Werror

all: verify

verify: verify.o
	$(CC) verify.o -o verify -lcrypto

verify.o: verify.c
	$(CC) $(CFLAGS) verify.c -lcrypto

clean:
	rm -f *.o verify
