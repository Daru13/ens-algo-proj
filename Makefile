CC = clang
CCFLAGS = -g -W -Wall -pedantic -ansi -std=c99

OBJS = fibonacci_heap.o toolbox.o

all: fibheap

fibheap : $(OBJS)
	$(CC) $(CCFLAGS) -o fibheap fibonacci_heap.c

fibonacci_heap.o : fibonacci_heap.c fibonacci_heap.h toolbox.h
	$(CC) $(CCFLAGS) -c fibonacci_heap.c

fibonacci_heap.h : toolbox.h

toolbox.o : toolbox.c toolbox.h
	$(CC) $(CCFLAGS) -c toolbox.c

clean:
	- rm fibheap
	- rm -f *.o
