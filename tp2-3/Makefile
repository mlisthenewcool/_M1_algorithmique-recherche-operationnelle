CC      = gcc
CCFLAGS = -g3 -Wall -std=c99
OBJECTS = util.o algo.o queue.o

all: main

main: src/main.c $(OBJECTS)
	$(CC) $(CCFLAGS) -o main src/main.c $(OBJECTS) -lm

%.o: src/%.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	rm -vf main *.o
