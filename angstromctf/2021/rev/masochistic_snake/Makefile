CC=gcc
CFLAGS=-O2
NAME=masochistic_snake
LIBS=-lncurses

.PHONY: build run

$(NAME): $(NAME).c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	strip $@

build: $(NAME)

run: build
	./$(NAME)
