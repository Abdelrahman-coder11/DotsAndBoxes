CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
OUTPUT = dotsandboxes
OBJS   = main.o game.o board.o

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^

main.o: main.c game.h
	$(CC) $(CFLAGS) -c $< -o $@
game.o: game.c game.h board.h
	$(CC) $(CFLAGS) -c $< -o $@
board.o: board.c board.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean
clean:
	rm -f $(OBJS) $(OUTPUT)
