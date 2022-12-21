CC=gcc
CFLAGS=-Wall -Wextra
LFAGS=
BIN=svece
OBJS=main.o
ARGS=Orhideja cvecare.txt
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
