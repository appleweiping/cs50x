CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

speller: speller.c dictionary.c dictionary.h
	$(CC) $(CFLAGS) -o speller speller.c dictionary.c

clean:
	rm -f speller speller.exe

.PHONY: clean
