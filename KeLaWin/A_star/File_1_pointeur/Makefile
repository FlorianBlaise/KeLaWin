CC=gcc
FLAGS=-Wall -Wextra -ansi -pedantic -g
I=-I include/

all: test_queue clean

test_queue: tests.o queue.o cell.o
	$(CC) -g $(I) $(FLAGS) $^ -o $@

tests.o: sources/tests.c
	$(CC) -g $(I) $(FLAGS) -c $^ -o $@

queue.o: sources/queue.c
	$(CC) -g $(I) $(FLAGS) -c $^ -o $@

cell.o: sources/cell.c
	$(CC) -g $(I) $(FLAGS) -c $^ -o $@

clean: 
	rm ./*.o