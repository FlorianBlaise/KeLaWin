
PILOT_NAME = KeLaWin

CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -Wpedantic -g
LDLIBS = -lm
INCLUDE_PATH = -I./include
SRC_PATH = ./src

.PHONY: all clean distclean install try

DRIVERS_PATH = ../drivers

all: $(PILOT_NAME)

install: $(DRIVERS_PATH)/$(PILOT_NAME)

try: install
	cd .. ; ./GrandPrix

$(DRIVERS_PATH)/$(PILOT_NAME): $(PILOT_NAME)
	cp $(PILOT_NAME) $(DRIVERS_PATH)

$(PILOT_NAME): $(PILOT_NAME).c $(SRC_PATH)/*.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $@ $^ $(LDLIBS)

distclean: clean
	rm -f $(PILOT_NAME)

clean:
	rm -f *~ *.o
