CC=gcc
CFLAGS=-Wall -Wextra
LFAGS=
BIN=main
OBJS=main.o
INPUT_FILE=automobili.txt
.PHONY: all build test clean

all: $(BIN)
	./$(BIN) $(INPUT_FILE)

build: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS)

test: 
	echo "No tests"

clean:
