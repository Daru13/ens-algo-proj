# DIJKSTRA WITH FIBONACCI HEAPS - 2016 Programming course project
# Some compiling-related parameters
CC = gcc
CCFLAGS = -g -W -Wall -pedantic -std=c99

##### THIS LIST MUST BE UPDATED #####
# List of all  object files which must be produced before any binary
OBJS = build/dijkstra.o build/fibonacci_heap.o build/graph.o build/list.o build/toolbox.o build/complexity.o 

# Dependencies and compiling rules
all: build/main_test build/naive build/optimized build/generator build/batch_test

test: build/main_test
naive: build/naive
optimized: build/optimized
generator: build/generator
batch: build/batch_test

build/optimized: src/main.c $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) src/main.c -DUSE_OPTIMIZED_VERSION -o build/optimized 

build/naive: src/main.c $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) src/main.c -o build/naive 

build/main_test: src/main_test.c src/main_test.h $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) src/main_test.c -o build/main_test

build/batch_test: src/batch_test.c $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) src/batch_test.c -o build/batch_test

build/generator: src/graph_gen.c src/graph_gen.h build/toolbox.o # $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -O3 src/graph_gen.c -o build/generator 

src/main_test.h: src/fibonacci_heap.h

build/complexity.o: src/complexity.c src/complexity.h
	$(CC) $(CCFLAGS) -O3 -Wno-unused-parameter -c src/complexity.c -o build/complexity.o

src/complexity.h: src/list.h src/graph.h src/fibonacci_heap.h src/dijkstra.h

build/dijkstra.o: src/dijkstra.c src/dijkstra.h src/toolbox.h src/list.h src/graph.h src/complexity.h
	$(CC) $(CCFLAGS) -c src/dijkstra.c -o build/dijkstra.o

src/dijkstra.h: src/graph.h 

build/fibonacci_heap.o: src/fibonacci_heap.c src/fibonacci_heap.h src/toolbox.h src/complexity.h
	$(CC) $(CCFLAGS) -c src/fibonacci_heap.c -o build/fibonacci_heap.o

src/fibonacci_heap.h: src/toolbox.h

build/graph.o: src/graph.c src/graph.h src/list.h src/toolbox.h src/complexity.h
	$(CC) $(CCFLAGS) -c src/graph.c -o build/graph.o

build/list.o: src/list.c src/list.h src/toolbox.h src/complexity.h
	$(CC) $(CCFLAGS) -c src/list.c -o build/list.o

build/toolbox.o: src/toolbox.c src/toolbox.h 
	$(CC) $(CCFLAGS) -c src/toolbox.c -o build/toolbox.o


# Cleaning rule(s)
clean:
	- rm build/main_test
	- rm build/naive
	- rm build/optimized
	- rm build/*.o
