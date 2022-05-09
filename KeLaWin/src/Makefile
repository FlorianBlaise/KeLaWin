CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -Wpedantic -g
LDLIBS = -lm
SRC = .
BIN = ../bin
INC = ../include/
OBJ = ../obj

.PHONY: all clean distclean test

all: main test

$(BIN)/main: $(OBJ)/main.o $(OBJ)/map.o $(OBJ)/path.o
	$(CC) $(CFLAGS) -o $@ $^ -I$(INC) $(LDLIBS)

$(BIN)/test: $(OBJ)/test.o $(OBJ)/map.o $(OBJ)/path.o
	$(CC) $(CFLAGS) $^ -I$(INC) -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -o $@ -c $< -I$(INC)

distclean:
	rm -f $(BIN)/*

clean:
	rm -f *~ *.o
