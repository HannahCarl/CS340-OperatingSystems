CC=gcc
CFLAGS=-c -Wall -g #-std=c11
SRCS=memory.c cache.c hw1.c
OBJS=memory.o cache.o hw1.o
# MAKE SURE TO PUT YOUR NAME HERE
TARBALL=carl_hw1.tar.gz

all: hw1

hw1:	$(OBJS)
	$(CC) memory.o cache.o hw1.o -o hw1 -lm

memory.o: memory.c
	$(CC) $(CFLAGS) memory.c

cache.o: cache.c
	$(CC) $(CFLAGS) cache.c

hw1.o: hw1.c
	$(CC) $(CFLAGS) hw1.c

clean:
	/bin/rm -f hw1 $(OBJS) $(TARBALL)

run:
	./hw1 memory.txt

tarball:
	tar cvzf $(TARBALL) $(SRCS) Makefile

