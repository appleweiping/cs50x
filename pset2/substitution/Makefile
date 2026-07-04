CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

substitution: substitution.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o substitution substitution.c ../../lib/cs50.c

clean:
	rm -f substitution substitution.exe

.PHONY: clean
