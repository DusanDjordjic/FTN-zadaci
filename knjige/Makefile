CC=gcc
CFLAGS=-Wall -Wextra
LFLAGS=
BIN=main
OBJS=main.o
ARGV=11.5 knjige.txt duzina_citanja.txt
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
