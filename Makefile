CC = gcc
CFLAGS = -g -std=c99 -Wall -Wvla

all: memgrind test

mymalloc: mymalloc.c
	$(CC) $(CFLAGS) -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o

test: mymalloc.c test.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o test test.c mymalloc.o

clean:
	rm -f mymalloc.o memgrind
	rm -f mymalloc.o test
