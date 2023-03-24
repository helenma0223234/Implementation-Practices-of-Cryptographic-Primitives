CC = gcc
CFLAGS = -c -Wall

all: privatekey

privatekey: privatekey.o
		$(CC) privatekey.o -o privatekey -lcrypto

privatekey.o: privatekey.c
		$(CC) $(CFLAGS) privatekey.c -lcryptto

clean:
		rm -f *.o privatekey
