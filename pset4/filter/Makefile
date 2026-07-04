CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

filter: filter.c helpers.c helpers.h bmp.h
	$(CC) $(CFLAGS) -o filter filter.c helpers.c -lm

clean:
	rm -f filter filter.exe

.PHONY: clean
