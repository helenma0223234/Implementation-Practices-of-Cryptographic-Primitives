CC = gcc
CFLAGS = -c -Wall

all: rsa

rsa: rsa.o
	$(CC) rsa.o -o rsa

rsa.o: rsa.c
	$(CC) $(CFLAGS) rsa.c

clean:
	rm -f *.o rsa
