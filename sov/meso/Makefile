CC=gcc
CFLAGS=-Wall -Wextra
LFAGS=
BIN=povoljno_meso
OBJS=main.o
ARGS=junetina cene_mesa.txt analiza.txt
.PHONY: all build test clean

all: $(BIN)
	./$(BIN) $(ARGS)

build: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS)

test: 
	echo "No tests"

clean:
	rm -rf *.o $(BIN)
