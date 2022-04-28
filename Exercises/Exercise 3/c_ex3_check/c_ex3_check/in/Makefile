CFLAGS = -Wvla -Wall -Wextra -g -std=c99
CC = gcc
AR = ar

all: ProductExample.o intsTreeCheck.o stringsTreeCheck.o vectorsTreeCheck.o RBTree.a Structs.a
	$(CC) -o ProductExample ProductExample.o RBTree.a -lm
	$(CC) -o intsTreeCheck intsTreeCheck.o RBTree.a -lm
	$(CC) -o stringsTreeCheck stringsTreeCheck.o RBTree.a Structs.o -lm
	$(CC) -o vectorsTreeCheck vectorsTreeCheck.o RBTree.a Structs.o -lm
	
ProductExample.o: ProductExample.c 
	$(CC) -c $(CFLAGS) ProductExample.c

intsTreeCheck.o: intsTreeCheck.c 
	$(CC) -c $(CFLAGS) intsTreeCheck.c

stringsTreeCheck.o: stringsTreeCheck.c 
	$(CC) -c $(CFLAGS) stringsTreeCheck.c

vectorsTreeCheck.o: vectorsTreeCheck.c 
	$(CC) -c $(CFLAGS) vectorsTreeCheck.c

RBTree.a: RBTree.o
	$(AR) rcs RBTree.a RBTree.o

Structs.a: Structs.o
	$(AR) rcs Structs.a Structs.o

RBTree.o: RBTree.c
	$(CC) -c $(CFLAGS) RBTree.c

Structs.o: Structs.c
	$(CC) -c $(CFLAGS) Structs.c

