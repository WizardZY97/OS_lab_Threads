CC=gcc
CFLAGS=-Wall #-Werror -g
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))

all: passing_objects barrier peru_and_bolivia clean

passing_objects: passing_objects.o
	$(CC) $(CFLAGS) -o $@ $< 

barrier: barrier.o
	$(CC) $(CFLAGS) -o $@ $< 

peru_and_bolivia: peru_and_bolivia.o
	$(CC) $(CFLAGS) -o $@ $< 

%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
.PHONY: clean
	
clean: 
	rm -rf *.o
