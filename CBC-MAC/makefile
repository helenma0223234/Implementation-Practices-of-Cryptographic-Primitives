CC = gcc
CFLAGS = -c -Wall

all: cbcmac

cbcmac: cbcmac.o desW23.o
	$(CC) cbcmac.o desW23.o -o cbcmac

cbcmac.o: cbcmac.c
	$(CC) $(CFLAGS) cbcmac.c

desW23.o: desW23.c
	$(CC) $(CFLAGS) desW23.c

clean:
	rm -f *.o cbcmac