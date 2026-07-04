CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

mario: mario.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o mario mario.c ../../lib/cs50.c

clean:
	rm -f mario mario.exe

.PHONY: clean
