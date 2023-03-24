CC = gcc
CFLAGS = -c -Wall

all: ctr cbc

ctr: ctr.o desW23.o
	$(CC) ctr.o desW23.o -o ctr

ctr.o: ctr.c
	$(CC) $(CFLAGS) ctr.c

cbc: cbc.o desW23.o
	$(CC) cbc.o desW23.o -o cbc

cbc.o: cbc.c
	$(CC) $(CFLAGS) cbc.c

desW23.o: desW23.c
	$(CC) $(CFLAGS) desW23.c

clean:
	rm -f *.o ctr cbc
