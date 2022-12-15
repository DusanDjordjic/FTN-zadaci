CC=gcc
CFLAGS=-Wall -Wextra
LFAGS=
BIN=main
OBJS=main.o
ARGV=letovi.txt LAX
.PHONY: all build test clean

all: $(BIN)
	./$(BIN) $(ARGV)

build: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS)

test: 
	echo "No tests"

clean:
	rm -rf *.o $(BIN)
