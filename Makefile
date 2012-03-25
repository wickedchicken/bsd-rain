# pick a CC
CC=clang
#CC=gcc

# CFLAGS="-Wall"
CFLAGS=-O2 -Wall

LIBS=-lcurses

LDFLAGS=$(LIBS)

#####################

all: rain

rain: rain.o
	$(CC) $(LDFLAGS) $< -o $@

rain.o: rain.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-rm -rf *.o rain
