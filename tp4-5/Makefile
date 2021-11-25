CC      = gcc
CCFLAGS = -g3 -Wall -std=c99

all: main

main: src/main.c
	$(CC) $(CCFLAGS) -o main src/main.c -lglpk -lm

.PHONY : clean

clean:
	rm -vf main *.o