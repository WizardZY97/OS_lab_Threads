CC=gcc
CFLAGS=-Wall #-Werror -g
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))
LIBS=-lpthread

all: passing_objects barrier peru_and_bolivia clean

passing_objects: passing_objects.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

barrier: barrier.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

peru_and_bolivia: peru_and_bolivia.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
.PHONY: clean
	
clean: 
	rm -rf *.o
