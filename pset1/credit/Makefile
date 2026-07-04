CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I../../lib

credit: credit.c ../../lib/cs50.c
	$(CC) $(CFLAGS) -o credit credit.c ../../lib/cs50.c

clean:
	rm -f credit credit.exe

.PHONY: clean
