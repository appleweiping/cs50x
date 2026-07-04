CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

tideman: tideman.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o tideman tideman.c ../../lib/cs50.c

clean:
	rm -f tideman tideman.exe

.PHONY: clean
