CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

plurality: plurality.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o plurality plurality.c ../../lib/cs50.c

clean:
	rm -f plurality plurality.exe

.PHONY: clean
