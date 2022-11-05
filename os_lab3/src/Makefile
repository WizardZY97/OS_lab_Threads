CC=gcc
CFLAGS=-Wall -Werror
SOURCES=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SOURCES));
LIBS=-lpthread

.PHONY: clean

all: match matchp counting generate_random_file Dependencies_between_threads clean

Dependencies_between_threads: Dependencies_between_threads.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
match: match.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

matchp: matchp.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

counting: counting.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

generate_random_file: generate_random_file.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f *.o
