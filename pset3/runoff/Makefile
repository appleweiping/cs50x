CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

runoff: runoff.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o runoff runoff.c ../../lib/cs50.c

clean:
	rm -f runoff runoff.exe

.PHONY: clean
