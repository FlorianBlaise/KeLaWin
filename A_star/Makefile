CC=gcc
FLAGS=-I./include -Wall -Wextra -ansi -pedantic


all: clean test cell

#main: L_ADJ.o #test.o
#	$(CC) -g  $(FLAGS) $^ -o $@

#L_ADJ.o: src/L_ADJ.c
#	$(CC) -g  $(FLAGS) -c $^ -o $@

test: src/test.c
	$(CC) $(FLAGS)  $^ -o ./bin/$@

cell: src/cell.c
	$(CC) $(FLAGS)  $^ -o ./bin/$@

clean:
	rm ./bin/*
	