CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=gnu89

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

hsh: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f hsh *.o

