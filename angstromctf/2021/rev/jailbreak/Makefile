CC=gcc
CFLAGS=-O2
NAME=jailbreak

.PHONY: build run

encstrings.h: genStrings.js
	node genStrings.js

$(NAME): $(NAME).c encstrings.h
	$(CC) -o $@ $< $(CFLAGS) -I.
	strip $@

build: $(NAME)

run: build
	./$(NAME)
