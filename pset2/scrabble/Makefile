CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

scrabble: scrabble.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o scrabble scrabble.c ../../lib/cs50.c

clean:
	rm -f scrabble scrabble.exe

.PHONY: clean
