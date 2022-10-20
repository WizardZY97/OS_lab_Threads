CC=gcc
CFLAGS=-Wall -Werror
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES))

all: peru_and_bolivia clean

peru_and_bolivia: peru_and_bolivia.o
	$(CC) $(CFLAGS) -o $@ $< 

%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
.PHONY: clean
	
clean: 
	rm -rf *.o
