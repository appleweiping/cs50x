CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

cash: cash.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o cash cash.c ../../lib/cs50.c -lm

clean:
	rm -f cash cash.exe

.PHONY: clean
