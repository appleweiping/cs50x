CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

readability: readability.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o readability readability.c ../../lib/cs50.c -lm

clean:
	rm -f readability readability.exe

.PHONY: clean
