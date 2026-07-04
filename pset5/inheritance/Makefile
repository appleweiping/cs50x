CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

inheritance: inheritance.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o inheritance inheritance.c ../../lib/cs50.c

clean:
	rm -f inheritance inheritance.exe

.PHONY: clean
