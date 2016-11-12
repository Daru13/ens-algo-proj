# DIJKSTRA WITH FIBONACCI HEAPS - 2016 Programming course project
# Some compiling-related parameters
CC = clang
CCFLAGS = -g -W -Wall -pedantic -std=c99

##### THIS LIST MUST BE UPDATED #####
# List of all  object files which must be produced before any binary
OBJS = build/fibonacci_heap.o build/toolbox.o

# Dependencies and compiling rules
all: build/main_test

build/main_test: $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) src/main_test.c -o build/main_test 

build/main_test.o : src/main_test.c src/main_test.h src/fibonacci_heap.c src/fibonacci_heap.h src/toolbox.h
	$(CC) $(CCFLAGS) -c src/main_test.c -o build/main_test.o

main_test.h : src/fibonacci_heap.h

build/fibonacci_heap.o : src/fibonacci_heap.c src/fibonacci_heap.h src/toolbox.h
	$(CC) $(CCFLAGS) -c src/fibonacci_heap.c -o build/fibonacci_heap.o

fibonacci_heap.h : src/toolbox.h

build/toolbox.o : src/toolbox.c src/toolbox.h
	$(CC) $(CCFLAGS) -c src/toolbox.c -o build/toolbox.o

# Cleaning rule(s)
clean:
	- rm build/main_test
	- rm build/*.o
