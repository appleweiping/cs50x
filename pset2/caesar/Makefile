CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

caesar: caesar.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o caesar caesar.c ../../lib/cs50.c

clean:
	rm -f caesar caesar.exe

.PHONY: clean
